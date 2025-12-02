const char SetupPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Thiết lập Wi-Fi</title>
  <style>
    body {
      background-color: #2c3e50;
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      color: #ecf0f1;
      display: flex;
      justify-content: center;
      padding-top: 30px;
    }

    #setupPage {
      background-color: #34495e;
      border-radius: 10px;
      padding: 25px 20px;
      width: 100%;
      max-width: 420px;
      box-shadow: 0 0 20px rgba(0, 0, 0, 0.4);
    }

    h1 {
      text-align: center;
      font-size: 22px;
      margin-bottom: 20px;
    }

    label {
      font-size: 14px;
      display: block;
      margin-top: 15px;
      margin-bottom: 5px;
    }

    input[type="text"],
    input[type="password"],
    select {
      width: 100%;
      padding: 8px;
      margin-bottom: 10px;
      border-radius: 5px;
      border: none;
      background-color: #2c3e50;
      color: #ecf0f1;
      font-size: 15px;
      box-sizing: border-box;
    }

    .checkbox {
      font-size: 13px;
      margin-top: -5px;
      margin-bottom: 10px;
    }

    .button-row {
      display: flex;
      justify-content: space-between;
      margin-top: 20px;
      flex-wrap: wrap;
      gap: 10px;
    }

    button {
      flex: 1;
      padding: 10px;
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

    #statusMessage {
      background-color: #27ae60;
      padding: 10px;
      border-radius: 5px;
      text-align: center;
      margin-top: 15px;
      display: none;
      font-size: 14px;
      color: white;
    }

    #errorMessage {
      background-color: #e74c3c;
      padding: 10px;
      border-radius: 5px;
      text-align: center;
      margin-top: 15px;
      display: none;
      font-size: 14px;
      color: white;
    }

    .footer, .help-note {
      text-align: center;
      font-size: 12px;
      color: #bdc3c7;
      margin-top: 20px;
    }

    .help-note a {
      color: #1abc9c;
      text-decoration: none;
      font-weight: bold;
    }
  </style>
</head>
<body>
  <div id="setupPage">
    <h1>THIẾT LẬP CẤU HÌNH</h1>

    <div>
      <strong>Chế độ phát Wi-Fi</strong>
      <label for="ssid_ap">Tên Wi-Fi phát:</label>
      <input id="ssid_ap" type="text" />

      <label for="pass_ap">Mật khẩu:</label>
      <input id="pass_ap" type="password" />
      <div class="checkbox">
        <input type="checkbox" onclick="showpass('ap')"> Hiện mật khẩu
      </div>
    </div>

    <div>
      <strong>Chế độ kết nối Wi-Fi</strong>
      <label for="ssid_sta">Tên Wi-Fi kết nối:</label>
      <input id="ssid_sta" type="text" />
      <select id="ssidList" onchange="updatessid_sta()">
        <option>Chọn mạng Wi-Fi</option>
      </select>

      <label for="pass_sta">Mật khẩu:</label>
      <input id="pass_sta" type="password" />
      <div class="checkbox">
        <input type="checkbox" onclick="showpass('sta')"> Hiện mật khẩu
      </div>

      <label for="auth_token">Mã Auth Token:</label>
      <input id="auth_token" type="text" />
    </div>

    <div class="button-row">
      <button onclick="saveConfig()">Lưu</button>
      <button onclick="reStart()">Khởi động lại</button>
      <button onclick="window.location.href='/home'">Thoát</button>
    </div>

    <div id="statusMessage">Đã lưu thành công</div>
    <div id="errorMessage"></div>

    <div class="help-note">
      <strong>Bạn gặp khó khăn khi sử dụng?</strong><br>
      Vui lòng gọi tới số <a href="tel:0522216999">0522 216 999</a> (giờ hành chính).
    </div>

    <div class="footer">© 2025 Hệ thống máy ấp trứng thông minh</div>
  </div>

  <script>
    window.onload = function () {
      if (sessionStorage.getItem("LOGIN_STATE") !== "1") {
        window.location.assign('/login');
      } else {
        getConfig();
      }
    };

    function create_obj() {
      return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
    }

    const xhttp = create_obj();

    function getConfig() {
      xhttp.onreadystatechange = function () {
        if (xhttp.readyState === 4 && xhttp.status === 200) {
          const obj = JSON.parse(xhttp.responseText);
          const select = document.getElementById("ssidList");

          obj.ssidList.forEach(ssid => {
            const option = new Option(ssid, ssid);
            select.add(option);
          });

          document.getElementById("ssid_ap").value = obj.ssid_ap;
          document.getElementById("pass_ap").value = obj.pass_ap;
          document.getElementById("ssid_sta").value = obj.ssid_sta;
          document.getElementById("pass_sta").value = obj.pass_sta;
          document.getElementById("auth_token").value = obj.auth_token;
        }
      };

      xhttp.open("GET", "/getConfig", true);
      xhttp.send();
    }

    function updatessid_sta() {
      document.getElementById("ssid_sta").value = document.getElementById("ssidList").value;
    }

    function saveConfig() {
      const ssid_ap = document.getElementById("ssid_ap").value.trim();
      const pass_ap = document.getElementById("pass_ap").value.trim();
      const ssid_sta = document.getElementById("ssid_sta").value.trim();
      const pass_sta = document.getElementById("pass_sta").value.trim();
      const auth_token = document.getElementById("auth_token").value.trim();

      if (!ssid_ap) {
        showError("Vui lòng nhập tên Wi-Fi ở chế độ phát!");
        return;
      }

      const query = `/saveConfig?ssid_ap=${ssid_ap}&&pass_ap=${pass_ap}&&ssid_sta=${ssid_sta}&&pass_sta=${pass_sta}&&auth_token=${auth_token}`;
      xhttp.onreadystatechange = function () {
        if (xhttp.readyState === 4 && xhttp.status === 200) {
          showStatus("Đã lưu thành công");
        }
      };
      xhttp.open("GET", query, true);
      xhttp.send();
    }

    function reStart() {
      if (confirm("Bạn có muốn khởi động lại thiết bị hay không?")) {
        showStatus("Hệ thống đang khởi động lại...");
        xhttp.open("GET", "/reStart", true);
        xhttp.send();
      }
    }

    function showpass(mode) {
      const field = document.getElementById(mode === 'ap' ? "pass_ap" : "pass_sta");
      field.type = field.type === "password" ? "text" : "password";
    }

    function showStatus(message) {
      const statusBox = document.getElementById("statusMessage");
      statusBox.innerText = message;
      statusBox.style.display = "block";
      setTimeout(() => { statusBox.style.display = "none"; }, 3000);
    }

    function showError(message) {
      const errorBox = document.getElementById("errorMessage");
      errorBox.innerText = message;
      errorBox.style.display = "block";
      setTimeout(() => { errorBox.style.display = "none"; }, 4000);
    }
  </script>
</body>
</html>
</html>
)=====";
