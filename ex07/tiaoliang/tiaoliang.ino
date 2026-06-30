#include <WiFi.h>

// WiFi配置
const char* ssid     = "7ing";
const char* password = "123456789";

#define LED_PIN 2
int ledBright = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, ledBright);

  // 连接WiFi
  Serial.print("连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("控制网页地址：http://");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // 解析滑动条传来的亮度数值
  if (req.indexOf("/?bright=") != -1) {
    int pos = req.indexOf("bright=");
    String valStr = req.substring(pos + 7, req.indexOf(" ", pos));
    ledBright = valStr.toInt();
    ledBright = constrain(ledBright, 0, 255);
    analogWrite(LED_PIN, ledBright);
  }

  // 网页HTML（包含题目要求的range滑动条+JS fetch请求）
  String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 网页无极调光器</title>
<style>
body{text-align:center;font-size:22px;margin-top:50px;background:#f5f5f5;}
#slider{width:85%;height:32px;margin:40px 0;}
#showVal{font-size:36px;color:#c41e3a;font-weight:bold;}
</style>
</head>
<body>
<h2>LED无极亮度调节</h2>
<!-- 题目指定滑动条标签 -->
<input type="range" id="slider" min="0" max="255" value="0">
<div>当前亮度：<span id="showVal">0</span></div>

<script>
const slider = document.getElementById("slider");
const showVal = document.getElementById("showVal");
// 监听滑动变化
slider.addEventListener("input", function(){
    let bri = this.value;
    showVal.innerText = bri;
    // fetch发送GET请求传递数值
    fetch(`/?bright=${bri}`);
});
</script>
</body>
</html>
)HTML";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);

  delay(1);
  client.stop();
}