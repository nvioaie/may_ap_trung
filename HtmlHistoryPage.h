const char HistoryPage[] PROGMEM = R"=====( 
  <!DOCTYPE html>
  <html lang="vi">
  <head>
    <meta charset="UTF-8">
    <title>LỊCH SỬ DỮ LIỆU</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      body {
        background-color: #2c3e50;
        font-family: Arial, sans-serif;
        color: white;
        margin: 0;
        padding: 0;
      }
  
      .container {
        max-width: 420px;
        margin: 40px auto;
        padding: 20px;
        background-color: #34495e;
        border-radius: 12px;
        box-shadow: 0 0 10px rgba(0,0,0,0.3);
      }
  
      h2 {
        text-align: center;
        margin-bottom: 20px;
      }
  
      table {
        width: 100%;
        border-collapse: collapse;
        margin-bottom: 15px;
      }
  
      th, td {
        padding: 8px;
        text-align: center;
        border-bottom: 1px solid #7f8c8d;
        font-size: 14px;
      }
  
      th {
        background-color: #1abc9c;
        color: white;
      }
  
      tr:nth-child(even) {
        background-color: #2c3e50;
      }
  
      tr:nth-child(odd) {
        background-color: #3d566e;
      }
  
      button {
        width: 100%;
        padding: 10px;
        background-color: #1abc9c;
        border: none;
        border-radius: 6px;
        color: white;
        font-weight: bold;
        font-size: 15px;
        cursor: pointer;
      }
  
      button:active {
        background-color: #16a085;
      }
  
      .footer {
        text-align: center;
        margin-top: 20px;
        font-size: 13px;
        color: #95a5a6;
      }
  
      .footer span {
        color: #2ecc71;
        font-weight: bold;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h2>LỊCH SỬ NHIỆT ĐỘ & ĐỘ ẨM</h2>
      <table>
        <thead>
          <tr>
            <th>Thời gian</th>
            <th>Nhiệt độ (°C)</th>
            <th>Độ ẩm (%)</th>
          </tr>
        </thead>
        <tbody id="historyTable">
          <tr><td colspan="3">Đang tải dữ liệu...</td></tr>
        </tbody>
      </table>
      <button onclick="loadHistory()">Làm mới</button>
  
      <div class="footer">
        Bạn gặp khó khăn khi sử dụng?<br>
        Vui lòng gọi tới số <span>0522 216 999</span>.<br><br>
        © 2025 Hệ thống máy ấp trứng thông minh
      </div>
    </div>
  
    <script>
      function loadHistory(){
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function(){
          if(xhttp.readyState==4 && xhttp.status==200){
            const lines = xhttp.responseText.trim().split('\n');
            let html = "";
            lines.reverse().forEach(line => {
              const [time, temp, humi] = line.split(',');
              html += `<tr><td>${time}</td><td>${temp}</td><td>${humi}</td></tr>`;
            });
            document.getElementById("historyTable").innerHTML = html || "<tr><td colspan='3'>Không có dữ liệu.</td></tr>";
          }
        };
        xhttp.open("GET", "/history.csv", true);
        xhttp.send();
      }
  
      window.onload = loadHistory;
    </script>
  </body>
  </html>
  )=====";
  