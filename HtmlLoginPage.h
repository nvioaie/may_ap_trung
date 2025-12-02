const char LoginPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Đăng nhập hệ thống</title>
  <style>
    body {
      background-color: #2c3e50;
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      color: #ecf0f1;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    #loginpage {
      background-color: #34495e;
      border-radius: 10px;
      padding: 30px 20px;
      width: 100%;
      max-width: 360px;
      box-shadow: 0 0 20px rgba(0,0,0,0.4);
    }

    h1 {
      text-align: center;
      margin-bottom: 20px;
      font-size: 24px;
    }

    label {
      display: block;
      margin-top: 15px;
      margin-bottom: 5px;
      font-size: 14px;
    }

    input[type="text"],
    input[type="password"] {
      width: 100%;
      padding: 10px;
      border: none;
      border-radius: 5px;
      box-sizing: border-box;
    }

    .checkbox-wrapper {
      margin-top: 8px;
      font-size: 13px;
    }

    .button-row {
      margin-top: 20px;
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
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

    #login_update {
      display: none;
    }

    .footer {
      text-align: center;
      margin-top: 30px;
      font-size: 12px;
      color: #bdc3c7;
    }

    .help-note {
      margin-top: 20px;
      font-size: 13px;
      text-align: center;
      color: #ecf0f1;
      line-height: 1.5;
    }

    .help-note a {
      color: #1abc9c;
      text-decoration: none;
      font-weight: bold;
    }

    #error_message {
      color: #e74c3c;
      text-align: center;
      font-size: 14px;
      margin-top: 10px;
      display: none;
    }
  </style>
</head>
<body>
  <div id="loginpage">
    <h1>ĐĂNG NHẬP HỆ THỐNG</h1>

    <div id="login_system">
      <label for="user_login">Tên người dùng</label>
      <input id="user_login" type="text">

      <label for="pass_login">Mật khẩu</label>
      <input id="pass_login" type="password">
      <div class="checkbox-wrapper">
        <input onclick="showpass('login')" type="checkbox"> Hiện mật khẩu
      </div>

      <div class="button-row">
        <button onclick="login()">Đăng nhập</button>
        <button onclick="login_update()">Đổi mật khẩu</button>
      </div>

      <div id="error_message">Sai tên đăng nhập hoặc mật khẩu</div>
    </div>

    <div id="login_update">
      <h1>CẬP NHẬT TÀI KHOẢN</h1>

      <label for="user_login_update">Tên người dùng mới</label>
      <input id="user_login_update" type="text">

      <label for="pass_login_update">Mật khẩu mới</label>
      <input id="pass_login_update" type="password">

      <label for="pass_login_update2">Xác minh mật khẩu</label>
      <input id="pass_login_update2" type="password">
      <div class="checkbox-wrapper">
        <input onclick="showpass('login_update')" type="checkbox"> Hiện mật khẩu
      </div>

      <div class="button-row">
        <button onclick="save_login_update()">Lưu</button>
        <button onclick="reStart()">Khởi động lại</button>
        <button onclick="exit()">Thoát</button>
      </div>
    </div>

    <div class="help-note">
      <p><strong>Bạn gặp khó khăn khi sử dụng?</strong><br>
      Vui lòng gọi tới số <a href="tel:0522216999">0522 216 999</a> (giờ hành chính).</p>
    </div>
    <div class="footer">© 2025 Hệ thống máy ấp trứng thông minh</div>
  </div>

  <script>
    function showpass(mode) {
      let field;
      if (mode === 'login') {
        field = document.getElementById("pass_login");
      } else {
        field = document.getElementById("pass_login_update");
      }
      field.type = field.type === "password" ? "text" : "password";
    }

    function login_update() {
      document.getElementById("login_system").style.display = "none";
      document.getElementById("login_update").style.display = "block";
    }

    function login() {
      const user = document.getElementById("user_login").value.trim();
      const pass = document.getElementById("pass_login").value.trim();
      const errorMsg = document.getElementById("error_message");

      fetch(`/checkLogin?user=${user}&pass=${pass}`)
        .then(res => res.text())
        .then(data => {
          if (data === "OK") {
            sessionStorage.setItem("LOGIN_STATE", "1");
            window.location.href = "/home";
          } else {
            errorMsg.style.display = "block";
          }
        })
        .catch(err => {
          errorMsg.innerText = "Lỗi kết nối!";
          errorMsg.style.display = "block";
        });
    }

    function save_login_update() {
      const newUser = document.getElementById("user_login_update").value.trim();
      const newPass = document.getElementById("pass_login_update").value.trim();
      const confirmPass = document.getElementById("pass_login_update2").value.trim();

      if (newUser === "" || newPass === "") {
        alert("Vui lòng điền đầy đủ thông tin
