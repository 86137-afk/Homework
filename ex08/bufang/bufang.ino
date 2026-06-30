#include <WiFi.h>

// WiFi配置（已修复引号问题）
const char* ssid = "7ing";
const char* password = "123456789";

// 硬件引脚定义
#define TOUCH_PIN 4   // 触摸检测引脚T0 GPIO4
#define ALARM_LED 2   // 报警LED引脚GPIO2

// 系统全局状态
// 0 = 未布防(Disarm)  1 = 已布防(Arm)  2 = 锁定报警(Alarm)
int sysState = 0;
// 触摸防抖标记
bool touchLock = false;
// 触摸阈值，串口观察后微调
const int touchThreshold = 300;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ALARM_LED, OUTPUT);
  digitalWrite(ALARM_LED, LOW);

  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 0;
  // 最多等待30秒，防止卡死
  while (WiFi.status() != WL_CONNECTED && timeout < 60) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi连接成功！");
    Serial.print("网页控制地址：http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi连接失败，请核对账号密码，仅支持2.4GWiFi");
  }
  server.begin();
}

void loop() {
  // 1. 触摸检测逻辑
  int touchVal = touchRead(TOUCH_PIN);
  Serial.printf("Touch:%d 系统状态:%d\n", touchVal, sysState);

  // 仅【已布防】状态下触摸才触发报警
  if (sysState == 1 && touchVal < touchThreshold && !touchLock) {
    touchLock = true;
    sysState = 2; // 切换锁定报警状态
    Serial.println("触发入侵，进入报警锁定！");
  }
  // 松手解除触摸标记，不解除报警
  if (touchVal >= touchThreshold && touchLock) {
    touchLock = false;
  }

  // 2. 报警LED闪烁逻辑
  if (sysState == 2) {
    // 高频狂闪
    digitalWrite(ALARM_LED, HIGH);
    delay(80);
    digitalWrite(ALARM_LED, LOW);
    delay(80);
  } else {
    // 未报警状态LED常灭
    digitalWrite(ALARM_LED, LOW);
  }

  // 3. Web服务处理网页请求
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  Serial.println("收到网页请求：" + req);
  client.flush();

  // 网页按钮指令解析
  if (req.indexOf("/arm") != -1) {
    sysState = 1; // 布防
  }
  if (req.indexOf("/disarm") != -1) {
    sysState = 0; // 撤防，重置报警
  }

  // 4. 安防控制网页HTML
  String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>物联网安防报警器</title>
<style>
body{text-align:center;margin-top:60px;font-size:24px;background:#1a1a1a;color:#fff;}
button{width:220px;height:80px;font-size:26px;margin:30px 15px;border:none;border-radius:12px;cursor:pointer;}
#armBtn{background:#228B22;color:white;}
#disBtn{background:#dc143c;color:white;}
</style>
</head>
<body>
<h2>ESP32 安防报警主机</h2>
<p>当前系统状态：<span id="stateText">未布防</span></p>
<button id="armBtn" onclick="sendArm()">布防 ARM</button>
<button id="disBtn" onclick="sendDisarm()">撤防 DISARM</button>

<script>
// 布防请求
function sendArm(){
  fetch("/arm");
  document.getElementById("stateText").innerText = "已布防，入侵检测开启";
}
// 撤防请求
function sendDisarm(){
  fetch("/disarm");
  document.getElementById("stateText").innerText = "未布防，报警已重置";
}
</script>
</body>
</html>
)HTML";

  // 返回网页给浏览器
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  client.stop();
}