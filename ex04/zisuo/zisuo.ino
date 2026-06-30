// 定义触摸引脚 (T0对应GPIO4)
#define TOUCH_PIN 4
// 定义LED引脚 (ESP32 DevKit板载LED通常是GPIO2)
#define LED_PIN 2
// 中断模式设置：0为轮询模式，1为中断模式
#define EXT_ISR_MODE 0

// 阈值，需要通过串口监视器观察并调整
int threshold = 300; 
// 触摸值
int touchValue;
// LED状态标记
bool ledState = false;
// 触摸防抖标记，防止长按反复翻转
bool isTouched = false;

void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口稳定

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始LED熄灭
}

void loop() {
  touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch Value: ");
  Serial.println(touchValue);

  // 检测到触摸 且 当前没有标记正在触摸（防抖）
  if (touchValue < threshold && !isTouched) {
    ledState = !ledState; // 翻转LED状态
    digitalWrite(LED_PIN, ledState);
    isTouched = true;    // 标记已触发触摸
    delay(150);          // 短暂防抖，避免抖动多次触发
  }
  // 松开触摸，清除触摸标记，允许下次切换
  else if (touchValue >= threshold && isTouched) {
    isTouched = false;
  }
  
  delay(100);
}