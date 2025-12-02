#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL63Dd_C7QB"
#define BLYNK_TEMPLATE_NAME "MAYAPTRUNG"
#include <BlynkSimpleEsp32.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include <Wire.h>                 //Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h>    //Thư viện giao tiếp LCD theo chuẩn I2C
LiquidCrystal_I2C lcd(0x27,16,2); //Khai báo địa chỉ và thông tin LCD
#include "DHTesp.h"
DHTesp dht;

#include <TimeLib.h>
#include <EEPROM.h>
#include "HtmlHomePage.h"
#include "HtmlSetupPage.h"
#include "HtmlLoginPage.h"
#define DHTPIN 23
#define output1 25    //Bóng đèn
#define output2 14    //Phun sương
#define output3 27    //Quạt hút
#define output4 26    //Đảo trứng
#define led_warning 33//Đèn cảnh báo
#define bt_set 16
#define bt_up  17
#define bt_down 18
#define bt_ok 19
#define buzzer 32    
#define led_signal 2 
#define timeReconnectWL 300000L         //Thời gian reconnect wifi là 5 phút
#define timeReconnectBL 300000L         //Thời gian reconnect bynk là 5 phút
#define timeUpdateWeb 1000L             //Thời gian gửi socket lên web

WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);
String ssid_ap, pass_ap,user_login, pass_login;
String ssid_sta="", pass_sta="", auth_token="";
String uptimesString;
boolean ledConnectWeb=0;
boolean reStoreConfigState=0;
BlynkTimer  timer;
unsigned long times2;                   //Thời gian update socket lên web
unsigned long time_angiu=millis();
unsigned long time_angiu_set=millis();
unsigned long time_angiu_up=millis();
unsigned long time_angiu_down=millis();
unsigned long time_daotrung=millis();

float t,h;
float gioihan_tmin=37.5;  
float gioihan_tmax=38.5;
float gioihan_hmin=60;
float gioihan_hmax=80;
float giatrihieuchinh=0;
float muc_quanhiet=40;
float muc_quadoam=80;
int ngay=0;
int ngayover=0;
int tgdaotrung=0;
boolean setupMode=0;
int timerID1, timerID2, timerID3; 
boolean bt_setState=1;
boolean bt_upState=1;
boolean bt_downState=1;
boolean bt_okState=1;
boolean canhbao_quanhiet=0;
boolean canhbao_quangay=0;
boolean canhbao_quadoam=0;
unsigned long time_canhbao=millis();
byte setnum=1;
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
unsigned long timeBlink=millis();
WidgetLED LEDKETNOI(V0);
#define NHIETDO V1
#define DOAM    V2
#define NGAYCONLAI  V3
#define DEN     V4
#define PHUNSUONG V5
#define QUATHUT V6
#define NHIETDO_MIN V7
#define NHIETDO_MAX V8
#define NHIETDO_OVER V9
#define DOAM_MIN V10
#define DOAM_MAX V11
#define DOAM_OVER V12
#define HIEUCHINH V13
#define SONGAYAP  V14
#define TGDAOTRUNG V15
#define CANHBAO V16

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  pinMode(output1,OUTPUT);
  pinMode(output2,OUTPUT);
  pinMode(output3,OUTPUT);
  pinMode(led_warning,OUTPUT);
  pinMode(output4,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(led_signal,OUTPUT);
  pinMode(bt_set,INPUT_PULLUP);
  pinMode(bt_up,INPUT_PULLUP);
  pinMode(bt_down,INPUT_PULLUP);
  pinMode(bt_ok,INPUT_PULLUP);  
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(led_warning,LOW);
  digitalWrite(output4, LOW);
  digitalWrite(buzzer,LOW);
  digitalWrite(led_signal,HIGH);
  //-------------------------------------------------
  if(reStoreConfig()){
    reStoreConfigState=1;
    Serial.println();Serial.println("**==============================================**");
    Serial.println("**           SETUP DEVICE CONFIGURATION         **");
    Serial.println("**==============================================**");
    if(!checkConnectWifi()){
      startAccesspointMode();
    }else{
      if(auth_token!=""){
        Blynk.config(auth_token.c_str(),"blynk.cloud", 80);
        Blynk.connect();
      }
    }
  }else{
    startAccesspointMode();
  }
  startWebServer();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  //-------------------------------------------------------
  //dht.begin();
  dht.setup(23, DHTesp::DHT22);
  
  timerID1 = timer.setInterval(2000L, readSensor);
  setSyncInterval(60*60);
  timerID2 = timer.setInterval(60000L,readTime);
  Wire.begin(21,22);         //Khởi tạo chân kết nối I2C
  lcd.init();                //Khởi tạo LCD
  lcd.clear();               //Xóa màn hình
  lcd.backlight();           //Bật đèn nền cho LCD
  lcd.createChar(0, degree);   //Tạo ký tự lưu vào byte thứ 0
  lcd.setCursor(2,0);        //Cột 2, dòng 0
  lcd.print("Welcom to");    //Ghi chữ bắt đầu từ cột thứ 2 dòng 0
  lcd.setCursor(0,1);        //Cột 0, dòng thứ 1
  lcd.print("May ap trung!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP connected:");
  lcd.setCursor(1,1);
  lcd.print(WiFi.localIP());
  delay(3000);
  lcd.clear(); 
  homeDisplay();
}
void loop(){
  if(millis()-timeBlink>1000){
    if(LEDKETNOI.getValue()) LEDKETNOI.off();
    else LEDKETNOI.on();
    Blynk.virtualWrite(DEN,digitalRead(output1));
    Blynk.virtualWrite(PHUNSUONG,digitalRead(output2));
    Blynk.virtualWrite(QUATHUT,digitalRead(output3));
    Blynk.virtualWrite(NHIETDO_MIN,gioihan_tmin);
    Blynk.virtualWrite(NHIETDO_MAX,gioihan_tmax);
    Blynk.virtualWrite(NHIETDO_OVER,muc_quanhiet);
    Blynk.virtualWrite(DOAM_MIN,gioihan_hmin);
    Blynk.virtualWrite(DOAM_MAX,gioihan_hmax);
    Blynk.virtualWrite(DOAM_OVER,muc_quadoam);
    Blynk.virtualWrite(HIEUCHINH,giatrihieuchinh);
    Blynk.virtualWrite(SONGAYAP,ngay);
    Blynk.virtualWrite(TGDAOTRUNG,tgdaotrung);
    Blynk.virtualWrite(CANHBAO,digitalRead(led_warning));
    timeBlink=millis();
  }
  webServer.handleClient();
  webSocket.loop();
  if(Blynk.connected()){
    Blynk.run();
  }
  timer.run();
  //---------------------Uptime-----------------------------------------------------------------//
  String uptimeMn, uptimeHr, uptimeDy;
  unsigned long uptimes = millis();
  uptimeMn = int((uptimes/(1000*60))%60);
  uptimeHr = int((uptimes/(1000*60*60))%24);
  uptimeDy = int((uptimes/(1000*60*60*24))%365);
  uptimesString = uptimeDy + "d : " + uptimeHr +"h : " +uptimeMn+"m";
  //------------------------------------------------------------------
  //-----------Update socket lên web-----------------------------------------------------------//
  if(millis()-times2>timeUpdateWeb){
    updateWebserver();
    times2=millis();
  }
  //--------------------------------------------------
  if(tgdaotrung!=0){
    if(millis()-time_daotrung>tgdaotrung*60000){
      Serial.println("ON đảo trứng!");
      digitalWrite(output4,HIGH);
      timerID3 = timer.setTimeout(15000L,offdaoTrung);
      time_daotrung=millis();
    }
  }
  if(canhbao_quanhiet==1||canhbao_quangay==1||canhbao_quadoam==1){
    if(millis()-time_canhbao>200){
      time_canhbao=millis();
      beep();
      digitalWrite(led_warning,!digitalRead(led_warning));
    }
  }else{
    digitalWrite(led_warning,LOW);
  }
  if(digitalRead(bt_set)==LOW){
    if(bt_setState==1){
      bt_setState=0;   
      beep();
      if(setupMode==0){
        setupMode=1;
        lcd.clear(); 
        setnum=1;   
      }else{
        setupMode=0;
        homeDisplay();
        save_setup();
      }
      Serial.print("Mode: ");Serial.println(setupMode);
    }
    if(millis()-time_angiu_set>3000){
      for(int i=0;i<30;++i){
        digitalWrite(led_signal,!digitalRead(led_signal));
        delay(200);
      }
      resetDefault();
      ESP.restart();
    }
  }else{
    bt_setState=1;
    time_angiu_set=millis();
  }
  if(setupMode==1){
    if(digitalRead(bt_ok)==LOW){
      if(bt_okState==1){
        beep();
        bt_okState=0;
        lcd.clear(); 
        if(setnum<10){
          setnum++;
        }else{
          setnum=1;
        }
        Serial.print("Set num: "); Serial.println(setnum);
      }
    }else{
      bt_okState=1;
    }
    if(setnum==1){
      lcd.setCursor(0,0);        
      lcd.print("IP cau hinh:");
      lcd.setCursor(1,1);
      lcd.print(WiFi.localIP());
    }else if(setnum==2){
      lcd.setCursor(0,0);        
      lcd.print("Nhiet do Min: ");
      lcd.setCursor(0,1); 
      gioihan_tmin = setupValue(gioihan_tmin,30.0, 40.0);     
      lcd.print(gioihan_tmin,1);
      lcd.write(0);
      lcd.print("C");
      lcd.setCursor(9,1);
      lcd.print("30<T<40");
    }else if(setnum==3){
      lcd.setCursor(0,0);        
      lcd.print("Nhiet do Max: ");
      lcd.setCursor(0,1);  
      gioihan_tmax = setupValue(gioihan_tmax,35.0, 45.0);    
      lcd.print(gioihan_tmax,1);    
      lcd.write(0);
      lcd.print("C");
      lcd.setCursor(9,1);
      lcd.print("35<T<45");
    }else if(setnum==4){
      lcd.setCursor(0,0);        
      lcd.print("Nhiet do Over: ");
      lcd.setCursor(0,1);  
      muc_quanhiet = setupValue(muc_quanhiet,40.0, 50.0);      
      lcd.print(muc_quanhiet,1);
      lcd.write(0);
      lcd.print("C");
      lcd.setCursor(9,1);
      lcd.print("40<T<50");
    }else if(setnum==5){
      lcd.setCursor(0,0);        
      lcd.print("Do am min: ");
      lcd.setCursor(0,1);  
      gioihan_hmin = setupValue2(gioihan_hmin,40, 80);     
      lcd.print(gioihan_hmin,0);
      lcd.print("% ");
      lcd.setCursor(9,1);
      lcd.print("40<H<80");
    }else if(setnum==6){
      lcd.setCursor(0,0);        
      lcd.print("Do am max: ");
      lcd.setCursor(0,1);  
      gioihan_hmax = setupValue2(gioihan_hmax,60, 80);     
      lcd.print(gioihan_hmax,0);
      lcd.print("% ");
      lcd.setCursor(8,1);
      lcd.print("60<H<80");
    }else if(setnum==7){
      lcd.setCursor(0,0);        
      lcd.print("Do am over: ");
      lcd.setCursor(0,1);  
      muc_quadoam = setupValue2(muc_quadoam,80, 100);     
      lcd.print(muc_quadoam,0);
      lcd.print("% ");
      lcd.setCursor(8,1);
      lcd.print("80<H<100");
    }else if(setnum==8){
      lcd.setCursor(0,0);        
      lcd.print("So ngay ap: ");
      lcd.setCursor(0,1);  
      ngay = setupValue2(ngay,15, 45);     
      lcd.print(ngay);
      lcd.print("  ");
      lcd.setCursor(9,1);
      lcd.print("15<D<45");
    }else if(setnum==9){
      lcd.setCursor(0,0);        
      lcd.print("TG dao trung: ");
      lcd.setCursor(0,1);  
      tgdaotrung = setupValue2(tgdaotrung,0, 120);     
      lcd.print(tgdaotrung);
      lcd.print("m  ");
      lcd.setCursor(8,1);
      lcd.print("0<TG<120");
    }else if(setnum==10){
      lcd.setCursor(0,0);        
      lcd.print("Hieu Chinh T:");
      lcd.setCursor(0,1);
      giatrihieuchinh = setupValue(giatrihieuchinh,-10.0, 10.0);       
      lcd.print(giatrihieuchinh,1);
      lcd.write(0);
      lcd.print("C  ");
      lcd.setCursor(8,1);
      lcd.print("-10<T<10");
    }
  }else{
    if(digitalRead(bt_ok)==LOW){
      if(bt_okState==1){
        bt_okState=0;
        //Serial.println("Button Ok push!");
        if(canhbao_quangay==1){
          canhbao_quangay=0;
          ngayover=0;
          EEPROM.write(29,ngayover);
          EEPROM.commit();
          digitalWrite(led_warning,LOW);
        }
      }
      if(millis()-time_angiu>2000){
        time_angiu=millis();
        ngayover=0;
        EEPROM.write(29,ngayover);
        EEPROM.commit();
        digitalWrite(led_warning,LOW);
        beep();
      }
    }else{
      bt_okState=1;
      time_angiu=millis();
    }
  }
}
//==================CHƯƠNG TRÌNH CON=====================
void offdaoTrung(){
  Serial.println("OFF Đảo trứng!");
  digitalWrite(output4,LOW);
}
void resetDefault(){
  for(int i=0; i<512;++i){
    EEPROM.write(i,0);
    delay(10);
  }
  ssid_ap = "MAY AP TRUNG";
  pass_ap = "12345678";
  user_login = "admin";
  pass_login = "admin";
  auth_token = "";
  for(int i=0;i<ssid_ap.length();++i){
    EEPROM.write(100+i,ssid_ap[i]);
  }
  for(int i=0;i<pass_ap.length();++i){
    EEPROM.write(132+i,pass_ap[i]);
  }
  for(int i=0;i<user_login.length();++i){
    EEPROM.write(196+i,user_login[i]);
  }
  for(int i=0;i<pass_login.length();++i){
    EEPROM.write(228+i,pass_login[i]);
  }
  writeFloat(0,30.0);  
  writeFloat(4,45.0);
  writeFloat(8,40.0);
  writeFloat(12,80.0);
  writeFloat(16,0.0);
  writeFloat(20,50.0);
  writeFloat(24,100.0);
  EEPROM.write(28,45);
  EEPROM.write(30,5);
  EEPROM.commit();
  Serial.println("Reset default done!");
}
void save_setup(){
  writeFloat(0,gioihan_tmin);  
  writeFloat(4,gioihan_tmax);
  writeFloat(8,gioihan_hmin);
  writeFloat(12,gioihan_hmax);
  writeFloat(16,giatrihieuchinh);
  writeFloat(20,muc_quanhiet);
  writeFloat(24,muc_quadoam);
  EEPROM.write(28,ngay);
  EEPROM.write(30,tgdaotrung);
  EEPROM.commit();
  Serial.println("Đã lưu cài đặt");
}
boolean reStoreConfig(){
  gioihan_tmin=readFloat(0);  
  gioihan_tmax=readFloat(4);
  gioihan_hmin=readFloat(8);
  gioihan_hmax=readFloat(12);
  giatrihieuchinh=readFloat(16);
  muc_quanhiet=readFloat(20);
  muc_quadoam=readFloat(24);
  ngay = EEPROM.read(28);
  ngayover = EEPROM.read(29);
  tgdaotrung = EEPROM.read(30);
  //Serial.print("Ngày còn lại: "); Serial.println(ngay-ngayover);
  Serial.println(); 
  Serial.println("**==============================================**");
  Serial.println("Reading data from EEPROM memory...!");
  if(EEPROM.read(100)!=0){
    for(int i=100; i<132;++i){
      ssid_ap += char(EEPROM.read(i));
    }
    for(int i=132; i<196;++i){
      pass_ap += char(EEPROM.read(i));
    }
    for(int i=196; i<228;++i){
      user_login += char(EEPROM.read(i));
    }
    for(int i=228; i<260;++i){
      pass_login += char(EEPROM.read(i));
    }
    ssid_ap = ssid_ap.c_str();
    pass_ap = pass_ap.c_str();
    user_login = user_login.c_str();
    pass_login = pass_login.c_str();
    Serial.println("**----------------ACCESSPOINT MODE--------------**");
    Serial.print("SSID ACCESSPOINT: "); Serial.println(ssid_ap);
    Serial.print("PASSWORD: "); Serial.println(pass_ap);
    Serial.print("URL WEB SERVER: "); Serial.print(WiFi.softAPIP());
    Serial.print("USER LOGIN: "); Serial.println(user_login);
    Serial.print("PASSWORD: "); Serial.println(pass_login);
  }else{
    for(int i=0; i<512;++i){
      EEPROM.write(i,0);
      delay(10);
    }
    ssid_ap = "MAY AP TRUNG";
    pass_ap = "12345678";
    user_login = "admin";
    pass_login = "admin";
    for(int i=100;i<ssid_ap.length();++i){
      EEPROM.write(i,ssid_ap[i]);
    }
    for(int i=0;i<pass_ap.length();++i){
      EEPROM.write(132+i,pass_ap[i]);
    }
    for(int i=196; i<260;++i){
      EEPROM.write(i,0);
      delay(10);
    }
    for(int i=0;i<user_login.length();++i){
      EEPROM.write(196+i,user_login[i]);
    }
    for(int i=0;i<pass_login.length();++i){
      EEPROM.write(228+i,pass_login[i]);
    }
    EEPROM.commit();
  }
  if(EEPROM.read(260)!=0){
    for(int i=260; i<292;++i){
      ssid_sta += char(EEPROM.read(i));
    }
    ssid_sta = ssid_sta.c_str();
    Serial.println("**-----------------STATION MODE-----------------**");
    Serial.print("SSID STATION: "); Serial.println(ssid_sta);
    if(EEPROM.read(292)!=0){
      for(int i=292; i<356;++i){
        pass_sta += char(EEPROM.read(i));
      }
      pass_sta = pass_sta.c_str();
      Serial.print("PASSWORD: "); Serial.println(pass_sta);
    }
    if(EEPROM.read(356)!=0){
      for(int i=356; i<388;++i){
        auth_token += char(EEPROM.read(i));
      }
      auth_token = auth_token.c_str();
      Serial.print("AUTH TOKEN: "); Serial.println(auth_token);
    }
    return true;
  }else{
    Serial.println("**-----------------STATION MODE-----------------**");
    Serial.println("Station mode configuration has not been set up!");
    return false;
  }
}
boolean checkConnectWifi(){
  Serial.print("Connecting to a wifi network");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_sta.c_str(),pass_sta.c_str());
  int n=0;
  while(n<20){
    if(WiFi.status()==WL_CONNECTED){
      Serial.println();Serial.print("CONNECTED TO "); Serial.println(ssid_sta);
      Serial.print("IP CONNECTED: ");Serial.println(WiFi.localIP());
      return true;
    }
    n++;
    delay(500);
    Serial.print(".");
  }
  Serial.println();Serial.println("The wifi connection process is timed out!");
  return false;
}
void startAccesspointMode(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid_ap.c_str(),pass_ap.c_str(), 1, false,4);
  Serial.println("**----------ACCESSPOINT MODE ACTIVATED----------**");
  Serial.println("Please connect to the device's wifi to configure!");
  Serial.print("SSID ACCESSPOINT: ");Serial.println(ssid_ap);
  Serial.print("PASSWORD: "); Serial.println(pass_ap);
  Serial.print("URL WEB SERVER: "); Serial.println(WiFi.softAPIP());
}
//----------------------------------WEBSERVER-------------------------------------------------//
void startWebServer(){
  webServer.on("/",[]{
    String s = FPSTR(LoginPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/login",[]{
    String s = FPSTR(LoginPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/home",[]{
    String s = FPSTR(HomePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/setup",[]{
    String s = FPSTR(SetupPage);
    webServer.send(200,"text/html",s);
  });
  webServer.onNotFound([]() {
    String s = FPSTR(HomePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/getSystem",getSystem);
  webServer.on("/saveLogin",saveLogin);
  webServer.on("/getConfig",getConfig);
  webServer.on("/saveConfig",saveConfig);
  webServer.on("/getSetup",getSetup);
  webServer.on("/saveSetup",saveSetup);
  webServer.on("/reStart",reStart);
  
  webServer.begin();
}
void getSystem(){
  String s = "{\"user_login\": \""+ user_login +"\"," +
              "\"pass_login\": \""+ pass_login +"\"}";
  webServer.send(200,"application/json",s);
  //Serial.println(s);
}
void saveLogin(){
  user_login = webServer.arg("user_login");
  pass_login = webServer.arg("pass_login");
  for(int i=196; i<260;++i){
    EEPROM.write(i,0);
    delay(10);
  }
  for(int i=0;i<user_login.length();++i){
    EEPROM.write(196+i,user_login[i]);
  }
  for(int i=0;i<pass_login.length();++i){
    EEPROM.write(228+i,pass_login[i]);
  }
  EEPROM.commit();
  String s = "Thông tin đăng nhập hệ thống đã được lưu thành công!";
  webServer.send(200,"text/html",s);
}
void getConfig(){
  int n = WiFi.scanNetworks();
  delay(100);
  String ssidList = "[\"";                //"[wifi1,wifi2,wifi3]"
  for(int i=0;i<n;++i){
    ssidList+= WiFi.SSID(i)+ "\"";        //{"keyword1":"value","keyword2":"value","keyword3":["wifi1","wifi2",...]}
    if(i<(n-1)){
      ssidList += ",\"";
    }
  }
  ssidList += "]";
  String s = "{\"ssid_ap\": \""+ ssid_ap +"\"," +
              "\"pass_ap\": "+ pass_ap + "," +
              "\"ssid_sta\": \""+ ssid_sta + "\"," +
              "\"pass_sta\": \""+ pass_sta + "\"," +
              "\"auth_token\": \""+ auth_token + "\"," +
              "\"ssidList\": "+ ssidList + "}";
  webServer.send(200,"application/json",s);
  //Serial.println(s);
}
void saveConfig(){
  ssid_ap = webServer.arg("ssid_ap");
  pass_ap = webServer.arg("pass_ap");
  ssid_sta = webServer.arg("ssid_sta");
  pass_sta = webServer.arg("pass_sta");
  auth_token = webServer.arg("auth_token");
  for(int i=100; i<196;++i){
    EEPROM.write(i,0);
    delay(10);
  }
  for(int i=260; i<388;++i){
    EEPROM.write(i,0);
    delay(10);
  }
  for(int i=0;i<ssid_ap.length();++i){
    EEPROM.write(100+i,ssid_ap[i]);
  }
  for(int i=0;i<pass_ap.length();++i){
    EEPROM.write(132+i,pass_ap[i]);
  }
  for(int i=0;i<ssid_sta.length();++i){
    EEPROM.write(260+i,ssid_sta[i]);
  }
  for(int i=0;i<pass_sta.length();++i){
    EEPROM.write(292+i,pass_sta[i]);
  }
  for(int i=0;i<auth_token.length();++i){
    EEPROM.write(356+i,auth_token[i]);
  }
  EEPROM.commit();
  String s = "Cấu hình đã được lưu thành công. Vui lòng khởi động lại thiết bị!";
  webServer.send(200,"text/html",s);
}
void getSetup(){
  int h_min = gioihan_hmin;
  int h_max = gioihan_hmax;
  int h_over = muc_quadoam;
  String s = "{\"nhietdo_min\": \""+ String(gioihan_tmin) + "\"," +
                "\"nhietdo_max\": \""+ gioihan_tmax + "\"," +
                "\"nhietdo_over\": \""+ muc_quanhiet + "\"," +
                "\"doam_min\": \""+ h_min + "\"," +
                "\"doam_max\": \""+ h_max + "\"," +
                "\"doam_over\": \""+ h_over + "\"," +
                "\"songayap\": \""+ ngay + "\"," +
                "\"tgdaotrung\": \""+ tgdaotrung + "\"," +
                "\"hieuchinh\": \""+ giatrihieuchinh +"\"}";
  webServer.send(200,"application/json",s);
  //Serial.println(s);
}
void saveSetup(){
  gioihan_tmin = webServer.arg("t_min").toFloat();
  gioihan_tmax = webServer.arg("t_max").toFloat();
  muc_quanhiet = webServer.arg("t_over").toFloat();
  
  gioihan_hmin = webServer.arg("h_min").toInt();
  gioihan_hmax = webServer.arg("h_max").toInt();
  muc_quadoam = webServer.arg("h_over").toInt();
  giatrihieuchinh = webServer.arg("hieuchinh").toFloat();
  ngay = webServer.arg("songayap").toInt();
  tgdaotrung = webServer.arg("tgdaotrung").toInt();
  
  writeFloat(0,gioihan_tmin);  
  writeFloat(4,gioihan_tmax);
  writeFloat(8,gioihan_hmin);
  writeFloat(12,gioihan_hmax);
  writeFloat(16,giatrihieuchinh);
  writeFloat(20,muc_quanhiet);
  writeFloat(24,muc_quadoam);
  EEPROM.write(28,ngay);
  EEPROM.write(30,tgdaotrung);
  EEPROM.commit();
  String s = "Đã thiết lập!";
  webServer.send(200,"text/html",s);
}
void reStart(){
  ESP.restart();
}
void webSocketEvent(uint8_t num, WStype_t type,uint8_t * payload,size_t length){
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.println(payloadString);
}
void updateWebserver(){
  ledConnectWeb = !ledConnectWeb;
  String JSONtxt = "{\"ledconnect\": \""+ String(ledConnectWeb) +"\"," +
                    "\"uptimes\": \""+ uptimesString + "\"," +
                    "\"output1\": \""+ !digitalRead(output1) + "\"," +
                    "\"output2\": \""+ !digitalRead(output2) + "\"," +
                    "\"output3\": \""+ !digitalRead(output3) + "\"," +
                    "\"songayconlai\": \""+ (ngay-ngayover) + "\"," +
                    "\"nhietdo\": \""+ t + "\"," +
                    "\"doam\": \""+ h + "\"}";
  webSocket.broadcastTXT(JSONtxt);
  //Serial.println(JSONtxt);
}
void beep(){
  digitalWrite(buzzer,HIGH);
  delay(300);
  digitalWrite(buzzer,LOW);
}
float setupValue(float value, float min, float max){
  if(digitalRead(bt_up)==LOW){
    if(bt_upState==1){
      bt_upState=0;
      beep(); 
      if(value<max){
        value=value+0.1;
      }else{
        value=min;
      }
    }
    if(millis()-time_angiu_up>1000){
      if(value<max){
        value=value+0.1;
      }else{
        value=min;
      }
    }
  }else{
    bt_upState=1;
    time_angiu_up=millis();
  }
  if(digitalRead(bt_down)==LOW){
    if(bt_downState==1){
      bt_downState=0; 
      beep();
      if(value>min){
        value=value-0.1;
      }else{
        value=max;
      }
    }
    if(millis()-time_angiu_down>1000){
      if(value>min){
        value=value-0.1;
      }else{
        value=max;
      }
    }
  }else{
    bt_downState=1;
    time_angiu_down=millis();
  }
  return value;
}
float setupValue2(float value, float min, float max){
  if(digitalRead(bt_up)==LOW){
    if(bt_upState==1){
      bt_upState=0;
      beep(); 
      if(value<max){
        value=value+1;
      }else{
        value=min;
      }
    }
    if(millis()-time_angiu_up>1000){
      if(value<max){
        value=value+1;
      }else{
        value=min;
      }
    }
  }else{
    bt_upState=1;
    time_angiu_up=millis();
  }
  if(digitalRead(bt_down)==LOW){
    if(bt_downState==1){
      bt_downState=0; 
      beep();
      if(value>min){
        value=value-1;
      }else{
        value=max;
      }
    }
    if(millis()-time_angiu_down>1000){
      if(value>min){
        value=value-1;
      }else{
        value=max;
      }
    }
  }else{
    bt_downState=1;
    time_angiu_down=millis();
  }
  return value;
}
void homeDisplay(){
  lcd.clear();
  lcd.setCursor(0,0);        
  lcd.print("T: ");  
  lcd.setCursor(0,1);        
  lcd.print("H: ");  
  lcd.setCursor(11,0);        
  lcd.print("D_Off"); 
  lcd.setCursor(12,1);
  if(ngay<10){
    lcd.print("0"); 
    lcd.print(ngay-ngayover); 
  }else{
    lcd.print(ngay-ngayover);
  }
}
void readSensor()
{
  digitalWrite(led_signal,!digitalRead(led_signal));
  float temp_h;
  float temp_t;
//  temp_h = dht.readHumidity();
//  temp_t = dht.readTemperature(); 
  temp_h = dht.getHumidity();
  temp_t = dht.getTemperature(); 
  //h = 64.36;
  //t= 35.63 + giatrihieuchinh;
  //if (isnan(temp_h) || isnan(temp_t)) {
  if(dht.getStatusString()=="OK"){
    h=temp_h;
    t=temp_t + giatrihieuchinh;
    Serial.print("Nhiệt độ: ");Serial.println(t);
    Serial.print("Độ ẩm: ");Serial.println(h);
    Blynk.virtualWrite(NHIETDO,t);
    Blynk.virtualWrite(DOAM,h);
  }else{
    //Serial.println("Failed to read from DHT sensor!");
  }
  Blynk.virtualWrite(NGAYCONLAI,(ngay-ngayover));
  if(setupMode==0){
    lcd.setCursor(3,0);
    lcd.print(t,1);
    lcd.write(0);
    lcd.print("C");
    lcd.setCursor(3,1);
    lcd.print(h,0);
    lcd.print("%");
    lcd.setCursor(12,1);
    if(ngay<10){
      lcd.print("0"); 
      lcd.print(ngay-ngayover); 
    }else{
      lcd.print(ngay-ngayover);
    }
  } 
//  Serial.print("Temp: ");Serial.println(t);
//  Serial.print("Humi: ");Serial.println(h);
  if(t<gioihan_tmin){
    digitalWrite(output1,HIGH);//Bật đèn
  }else if(t>gioihan_tmax){
    digitalWrite(output1,LOW);//Tắt đèn nếu đạt
  }
  if(h<gioihan_hmin){
    digitalWrite(output2,HIGH);//Bật phun sương
  }else if(h>gioihan_hmax){
    digitalWrite(output2,LOW);
  }
  if(t>muc_quanhiet){
    digitalWrite(output1,LOW);//Tắt đèn
    digitalWrite(output3,HIGH);//Bật quạt hút
    canhbao_quanhiet=1;
  }else if(t<gioihan_tmax){
    canhbao_quanhiet=0;
    digitalWrite(output3,LOW);//Tắt quạt hút
  }
  if(h>muc_quadoam){
    digitalWrite(output2,LOW);//Tắt phun sương
    digitalWrite(output3,HIGH);//Bật quạt hút
    canhbao_quadoam=1;
  }else if(h<gioihan_hmax){
    canhbao_quadoam=0;
    digitalWrite(output3,LOW);//Tắt quạt hút
  }
}
void readTime(){
  if(minute()==0&&hour()==7){
    if(ngayover<ngay){
      ngayover++;
      EEPROM.write(29,ngayover);
      EEPROM.commit();
      canhbao_quangay=0;
    }else{
      String notifyText = "QUÁ NGÀY ẤP TRỨNG RỒI BẠN NHÉ!";
      canhbao_quangay=1;
    }
  }
}
//-------------Ghi dữ liệu kiểu float vào bộ nhớ EEPROM----------------------//
float readFloat(unsigned int addr){
  union{
    byte b[4];
    float f;
  }data;
  for(int i=0; i<4; i++){
    data.b[i]=EEPROM.read(addr+i);
  }
  return data.f;
}
void writeFloat(unsigned int addr, float x){
  union{
    byte b[4];
    float f;
  }data;
  data.f=x;
  for(int i=0; i<4;i++){
    EEPROM.write(addr+i,data.b[i]);
  }
}
BLYNK_WRITE(NHIETDO_MIN){
  gioihan_tmin = param.asFloat();
  Serial.println("Tmin: "+String(gioihan_tmin));
}
BLYNK_WRITE(NHIETDO_MAX){
  gioihan_tmax = param.asFloat();
  Serial.println("Tmax: "+String(gioihan_tmax));
}
BLYNK_WRITE(NHIETDO_OVER){
  muc_quanhiet = param.asFloat();
  Serial.println("Tover: "+String(muc_quanhiet));
}
BLYNK_WRITE(DOAM_MIN){
  gioihan_hmin = param.asFloat();
  Serial.println("Hmin: "+String(gioihan_hmin));
}
BLYNK_WRITE(DOAM_MAX){
  gioihan_hmax = param.asFloat();
  Serial.println("Hmax: "+String(gioihan_hmax));
}
BLYNK_WRITE(DOAM_OVER){
  muc_quadoam = param.asFloat();
  Serial.println("Hover: "+String(muc_quadoam));
}
BLYNK_WRITE(HIEUCHINH){
  giatrihieuchinh = param.asFloat();
  Serial.println("T hiệu chỉnh: "+String(giatrihieuchinh));
}
BLYNK_WRITE(SONGAYAP){
  ngay = param.asInt();
  Serial.println("So ngay ap: "+String(ngay));
}
BLYNK_WRITE(TGDAOTRUNG){
  tgdaotrung = param.asFloat();
  Serial.println("TG dao trung: "+String(tgdaotrung));
}