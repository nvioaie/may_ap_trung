const char ChartPage[] PROGMEM = R"=====( 
    <!DOCTYPE html>
    <html lang="vi">
    <head>
      <meta charset="UTF-8">
      <title>Biểu đồ nhiệt độ & độ ẩm</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: #2c3e50;
          color: white;
          padding: 20px;
          margin: 0;
        }
        .container {
          max-width: 700px;
          margin: auto;
        }
        h2 {
          text-align: center;
          margin-bottom: 20px;
        }
        canvas {
          background-color: white;
          border-radius: 8px;
          padding: 10px;
          width: 100%;
        }
        .footer {
          text-align: center;
          margin-top: 20px;
          font-size: 13px;
          color: #95a5a6;
        }
        .footer b {
          color: #2ecc71;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h2>BIỂU ĐỒ NHIỆT ĐỘ &amp; ĐỘ ẨM</h2>
        <canvas id="myChart" height="350"></canvas>
    
        <div class="footer">
          Bạn gặp khó khăn khi sử dụng?<br>
          Gọi <b>0522 216 999</b><br>
          &copy; 2025 Hệ thống máy ấp trứng thông minh
        </div>
      </div>
    
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
      <script>
        async function loadData() {
          const response = await fetch("/history.csv");
          const text = await response.text();
          const lines = text.trim().split("\n").reverse();
    
          const labels = [], temps = [], humis = [];
    
          lines.forEach(line => {
            const parts = line.split(",");
            if (parts.length === 3) {
              labels.push(parts[0]);
              temps.push(parseFloat(parts[1]));
              humis.push(parseFloat(parts[2]));
            }
          });
    
          const ctx = document.getElementById('myChart').getContext('2d');
          new Chart(ctx, {
            type: 'line',
            data: {
              labels: labels.slice(-50),
              datasets: [
                {
                  label: 'Nhiệt độ (°C)',
                  data: temps.slice(-50),
                  borderColor: '#e74c3c',
                  backgroundColor: 'rgba(231, 76, 60, 0.1)',
                  fill: true,
                  tension: 0.2,
                  pointRadius: 2
                },
                {
                  label: 'Độ ẩm (%)',
                  data: humis.slice(-50),
                  borderColor: '#3498db',
                  backgroundColor: 'rgba(52, 152, 219, 0.1)',
                  fill: true,
                  tension: 0.2,
                  pointRadius: 2
                }
              ]
            },
            options: {
              responsive: true,
              interaction: {
                mode: 'index',
                intersect: false
              },
              plugins: {
                legend: {
                  labels: { color: '#fff', boxWidth: 12 }
                },
                tooltip: {
                  enabled: true
                }
              },
              scales: {
                x: {
                  ticks: { color: '#fff', maxRotation: 45, autoSkip: true },
                  title: { display: true, text: 'Thời gian', color: '#fff' },
                  grid: { color: '#7f8c8d' }
                },
                y: {
                  ticks: { color: '#fff' },
                  title: { display: true, text: 'Giá trị', color: '#fff' },
                  grid: { color: '#7f8c8d' }
                }
              }
            }
          });
        }
    
        window.onload = loadData;
      </script>
    </body>
    </html>
    )=====";
    