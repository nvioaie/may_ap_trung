const char HomePage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Trang chủ hệ thống</title>
  <script src="https://www.gstatic.com/charts/loader.js"></script>
  <style>
    body {
      background-color: #2c3e50;
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      color: #ecf0f1;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      padding-top: 30px;
      min-height: 100vh;
    }

    #homepage {
      background-color: #34495e;
      border-radius: 10px;
      padding: 25px 20px;
      width: 100%;
      max-width: 600px;
      box-shadow: 0 0 20px rgba(0, 0, 0, 0.4);
    }

    h1 {
      text-align: center;
      margin-bottom: 20px;
      font-size: 22px;
    }

    .section {
      margin-bottom: 20px;
    }

    label {
      display: block;
      margin-bottom: 5px;
      font-size: 14px;
    }

    input[type="text"],
    input[type="number"] {
      width: 100%;
      padding: 8px;
      margin-bottom: 10px;
      border-radius: 5px;
      border: none;
      background-color: #2c3e50;
      color: #ecf0f1;
      box-sizing: border-box;
      font-size: 15px;
    }

    .grid-3 {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      text-align: center;
    }

    .led_output {
      width: 30px;
      height: 30px;
      border-radius: 50%;
      border: 2px solid white;
      background-color: #222;
      margin: auto;
    }

    .label-row {
      display: flex;
      justify-content: space-between;
      font-size: 13px;
      margin-bottom: 10px;
    }

    button {
      width: 100%;
      padding: 10px;
      margin-top: 10px;
      background-color: #1abc9c;
      border: none;
      border-radius: 5px;
      font-weight: bold;
      color: white;
      cursor: pointer;
      transition: background-color 0.2s ease;
    }

    button:hover {
      background-color: #16a085;
    }

    #statusMessage, #alertBox {
      padding: 10px;
      text-align: center;
      border-radius: 5px;
      margin-top: 15px;
      display: none;
      font-size: 14px;
    }

    #statusMessage {
      background-color: #27ae60;
      color: white;
    }

    #alertBox {
      background-color: #e74c3c;
      color: white;
      font-weight: bold;
    }

    .footer, .help-note {
      text-align: center;
      font-size: 12px;
      color: #bdc3c7;
      margin-top: 15px;
    }

    .gauges {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      gap: 20px;
    }

    .gauge-box {
      flex: 1 1 160px;
      min-width: 140px;
      height: 180px;
    }
  </style>
</head>
<body>
  <div id="homepage">
    <h1>TRANG CHỦ HỆ THỐNG</h1>

    <div class="section">
      <label>Biểu đồ nhiệt độ và độ ẩm</label>
      <div class="gauges">
        <div id="gauge_temp" class="gauge-box"></div>
        <div id="gauge_humi" class="gauge-box"></div>
      </div>
    </div>

    <div class="section">
      <label for="songayconlai">Số ngày còn lại</label>
      <input id="songayconlai" type="text" readonly>
    </div>

    <div class="section">
      <div class="label-row">
        <span>Đèn</span>
        <span>Phun sương</span>
        <span>Quạt hút</span>
      </div>
      <div class="grid-3">
        <input id="led_output1" class="led_output" type="button" />
        <input id="led_output2" class="led_output" type="button" />
        <input id="led_output3" class="led_output" type="button" />
      </div>
    </div>

    <div class="section">
      <label>Giới hạn nhiệt độ (°C)</label>
      <div class="grid-3">
        <input id="t_min" type="number" step="0.1" placeholder="Min">
        <input id="t_max" type="number" step="0.1" placeholder="Max">
        <input id="t_over" type="number" step="0.1" placeholder="Over">
      </div>
    </div>

    <div class="section">
      <label>Giới hạn độ ẩm (%)</label>
      <div class="grid-3">
        <input id="h_min" type="number" step="1" placeholder="Min">
        <input id="h_max" type="number" step="1" placeholder="Max">
        <input id="h_over" type="number" step="1" placeholder="Over">
      </div>
    </div>

    <div class="section">
      <label for="hieuchinh">Hiệu chỉnh nhiệt độ (°C)</label>
      <input id="hieuchinh" type="number" step="0.1">
    </div>

    <div class="section">
      <label for="songayap">Số ngày ấp trứng</label>
      <input id="songayap" type="number" step="1">
    </div>

    <div class="section">
      <label for="tgdaotrung">Thời gian đảo trứng (phút)</label>
      <input id="tgdaotrung" type="number" step="1">
    </div>

    <button onclick="window.location.href='/setup'">Cài đặt Wi-Fi</button>
    <button onclick="saveSetup()">Lưu thiết lập</button>

    <div id="statusMessage">Đã lưu thành công</div>
    <div id="alertBox"></div>

    <div class="help-note">
      <strong>Bạn gặp khó khăn khi sử dụng?</strong><br>
      Vui lòng gọi tới số <a href="tel:0522216999" style="color:#1abc9c;">0522 216 999</a> (giờ hành chính).
    </div>
    <div class="footer">© 2025 Hệ thống máy ấp trứng thông minh</div>
  </div>

  <script>
    google.charts.load('current', { 'packages': ['gauge'] });
    google.charts.setOnLoadCallback(drawGauge);

    let gaugeTemp, gaugeHumi, dataTemp, dataHumi, optionsTemp, optionsHumi;

    function drawGauge() {
      dataTemp = google.visualization.arrayToDataTable([['Label', 'Value'], ['Nhiệt độ', 36]]);
      optionsTemp = {
        width: 180, height: 180,
        redFrom: 40, redTo: 60,
        yellowFrom: 37, yellowTo: 40,
        greenFrom: 20, greenTo: 37,
        minorTicks: 5,
        max: 60
      };
      gaugeTemp = new google.visualization.Gauge(document.getElementById('gauge_temp'));
      gaugeTemp.draw(dataTemp, optionsTemp);

      dataHumi = google.visualization.arrayToDataTable([['Label', 'Value'], ['Độ ẩm', 60]]);
      optionsHumi = {
        width: 180, height: 180,
        redFrom: 80, redTo: 100,
        yellowFrom: 70, yellowTo: 80,
        greenFrom: 30, greenTo: 70,
        minorTicks: 5,
        max: 100
      };
      gaugeHumi = new google.visualization.Gauge(document.getElementById('gauge_humi'));
      gaugeHumi.draw(dataHumi, optionsHumi);
    }

    function updateGauge(temp, humi) {
      if (!dataTemp || !dataHumi) return;
      const tOver = parseFloat(document.getElementById("t_over").value || 100);
      const hOver = parseFloat(document.getElementById("h_over").value || 100);
      const alertBox = document.getElementById("alertBox");

      dataTemp.setValue(0, 1, parseFloat(temp));
      dataHumi.setValue(0, 1, parseFloat(humi));
      gaugeTemp.draw(dataTemp, optionsTemp);
      gaugeHumi.draw(dataHumi, optionsHumi);

      // Cảnh báo
      if (parseFloat(temp) > tOver) {
        showAlert("⚠️ Nhiệt độ vượt ngưỡng cho phép!");
      } else if (parseFloat(humi) > hOver) {
        showAlert("⚠️ Độ ẩm vượt ngưỡng cho phép!");
      }
    }

    function showAlert(msg) {
      const alertBox = document.getElementById("alertBox");
      alertBox.innerText = msg;
      alertBox.style.display = "block";
      setTimeout(() => { alertBox.style.display = "none"; }, 4000);
    }

    window.onload = function () {
      if (sessionStorage.getItem("LOGIN_STATE") !== "1") {
        window.location.assign('/login');
      } else {
        init();
        getSetup();
      }
    }

    let Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function (event) {
        const obj = JSON.parse(event.data);
        document.getElementById("songayconlai").value = obj.songayconlai + " ngày";
        document.getElementById("led_output1").style.background = obj.output1 == "0" ? "#23C48E" : "#222";
        document.getElementById("led_output2").style.background = obj.output2 == "0" ? "#23C48E" : "#222";
        document.getElementById("led_output3").style.background = obj.output3 == "0" ? "#23C48E" : "#222";

        updateGauge(obj.nhietdo, obj.doam);
      }
    }

    function create_obj() {
      return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
    }

    const xhttp = create_obj();

    function getSetup() {
      xhttp.onreadystatechange = function () {
        if (xhttp.readyState === 4 && xhttp.status === 200) {
          const obj = JSON.parse(xhttp.responseText);
          document.getElementById("t_min").value = obj.nhietdo_min;
          document.getElementById("t_max").value = obj.nhietdo_max;
          document.getElementById("t_over").value = obj.nhietdo_over;
          document.getElementById("h_min").value = obj.doam_min;
          document.getElementById("h_max").value = obj.doam_max;
          document.getElementById("h_over").value = obj.doam_over;
          document.getElementById("hieuchinh").value = obj.hieuchinh;
          document.getElementById("songayap").value = obj.songayap;
          document.getElementById("tgdaotrung").value = obj.tgdaotrung;
        }
      };
      xhttp.open("GET", "/getSetup", true);
      xhttp.send();
    }

    function saveSetup() {
      const params = {
        t_min: document.getElementById("t_min").value,
        t_max: document.getElementById("t_max").value,
        t_over: document.getElementById("t_over").value,
        h_min: document.getElementById("h_min").value,
        h_max: document.getElementById("h_max").value,
        h_over: document.getElementById("h_over").value,
        hieuchinh: document.getElementById("hieuchinh").value,
        songayap: document.getElementById("songayap").value,
        tgdaotrung: document.getElementById("tgdaotrung").value
      };
      const query = Object.entries(params).map(([k, v]) => `${k}=${v}`).join("&&");

      xhttp.onreadystatechange = function () {
        if (xhttp.readyState === 4 && xhttp.status === 200) {
          const msg = document.getElementById("statusMessage");
          msg.style.display = "block";
          msg.textContent = "Đã lưu thành công";
          setTimeout(() => { msg.style.display = "none"; }, 2000);
        }
      };
      xhttp.open("GET", `/saveSetup?${query}`, true);
      xhttp.send();
    }
  </script>
</body>
</html>
)=====";
