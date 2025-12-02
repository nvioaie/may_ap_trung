const char EditAccountPage[] PROGMEM = R"=====( 
    <!DOCTYPE html>
    <html lang="vi">
    <head>
      <meta charset="UTF-8">
      <title>Chỉnh sửa tài khoản</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: #2c3e50;
          color: white;
          display: flex;
          justify-content: center;
          align-items: center;
          height: 100vh;
        }
        .form-box {
          background-color: #34495e;
          padding: 30px;
          border-radius: 12px;
          box-shadow: 0 0 15px rgba(0,0,0,0.4);
          width: 320px;
        }
        h2 {
          text-align: center;
          margin-bottom: 20px;
        }
        label {
          display: block;
          margin-top: 10px;
          font-weight: bold;
        }
        input, select {
          width: 100%;
          padding: 10px;
          margin-top: 5px;
          border: none;
          border-radius: 6px;
          font-size: 14px;
        }
        button {
          margin-top: 20px;
          width: 100%;
          padding: 12px;
          background-color: #1abc9c;
          border: none;
          border-radius: 6px;
          font-weight: bold;
          font-size: 15px;
          color: white;
          cursor: pointer;
        }
        button:hover {
          background-color: #16a085;
        }
        .back-btn {
          background-color: #e67e22;
        }
        .back-btn:hover {
          background-color: #d35400;
        }
      </style>
    </head>
    <body>
      <div class="form-box">
        <h2>Chỉnh sửa tài khoản</h2>
        <form action="/saveLogin" method="get">
          <label for="user_login">Tên đăng nhập</label>
          <input name="user_login" id="user_login" placeholder="VD: admin" required>
    
          <label for="pass_login">Mật khẩu</label>
          <input name="pass_login" id="pass_login" placeholder="VD: 1234" required>
    
          <label for="role">Quyền truy cập</label>
          <select name="role" id="role">
            <option value="admin">Admin (Toàn quyền)</option>
            <option value="viewer">Viewer (Chỉ xem)</option>
          </select>
    
          <button type="submit">Lưu tài khoản</button>
        </form>
    
        <button class="back-btn" onclick="window.location.href='/home'">Thoát về Trang chủ</button>
      </div>
    </body>
    </html>
    )=====";
    