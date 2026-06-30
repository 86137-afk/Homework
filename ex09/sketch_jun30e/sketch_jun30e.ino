#include <WiFi.h>
#include <WebServer.h>

// WiFi配置
const char* ssid     = "7ing";
const char* password = "123456789";

WebServer server(80);
// 直接使用GPIO4，替代T0，解决未定义报错
const uint8_t touchPin = 4;
uint16_t touchVal = 0;

// Web仪表盘HTML页面（AJAX实时拉取触摸值）
const char* htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>ESP32触摸实时仪表盘</title>
    <style>
        *{margin:0;padding:0;box-sizing:border-box}
        body{
            background:#0f172a;
            color:#ffffff;
            font-family:Arial,sans-serif;
            height:100vh;
            display:flex;
            flex-direction:column;
            align-items:center;
            justify-content:center;
        }
        .title{
            font-size:28px;
            margin-bottom:40px;
            color:#38bdf8;
        }
        .meter-box{
            width:400px;
            height:300px;
            border:4px solid #38bdf8;
            border-radius:20px;
            display:flex;
            align-items:center;
            justify-content:center;
            background:#1e293b;
            box-shadow: 0 0 30px #0ea5e950;
        }
        #touchNum{
            font-size:90px;
            font-weight:bold;
            color:#22d3ee;
        }
        .tip{
            margin-top:30px;
            font-size:16px;
            color:#94a3b8;
        }
    </style>
</head>
<body>
    <div class="title">触摸传感器实时仪表</div>
    <div class="meter-box">
        <div id="touchNum">0</div>
    </div>
    <div class="tip">手指靠近GPIO4引脚数值变小，松开恢复</div>

    <script>
        // AJAX定时请求传感器数据
        function getTouchData(){
            fetch("/getTouch")
                .then(res => res.json())
                .then(data => {
                    document.getElementById("touchNum").innerText = data.value;
                })
                .catch(err => console.log("数据拉取失败",err));
        }
        // 200ms刷新一次，实时显示
        setInterval(getTouchData, 200);
    </script>
</body>
</html>
)HTML";

// 下发网页页面路由
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 传感器数据上报API接口
void handleGetTouch() {
  String json = "{\"value\":" + String(touchVal) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("设备访问IP：");
  Serial.println(WiFi.localIP());

  // 注册网页与数据接口路由
  server.on("/", handleRoot);
  server.on("/getTouch", handleGetTouch);
  server.begin();
  Serial.println("Web仪表盘服务已启动");
}

void loop() {
  server.handleClient(); // 持续处理浏览器请求
  touchVal = touchRead(touchPin); // 采集触摸模拟量（数据上报源）
  delay(100);
}