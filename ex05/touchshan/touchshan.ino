// 硬件引脚定义
#define TOUCH_PIN 4    // T0触摸通道 GPIO4
#define LED_PIN 2      // ESP32板载LED

// 触摸阈值，串口查看数值后自行微调
int touchThreshold = 300;
int touchValue;
bool touchLock = false; // 触摸防抖标记

// 呼吸灯参数
int ledBright = 0;        // LED亮度 0~255
int baseStep;             // 档位基础步长（控制快慢）
int dir = 1;              // 渐变方向：1=变亮，-1=变暗
// 三档速度：1慢 2中 3快
const int step1 = 1;
const int step2 = 3;
const int step3 = 6;

int speedGear = 1;        // 档位 1/2/3循环

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 1. 触摸检测与档位切换
  touchValue = touchRead(TOUCH_PIN);
  Serial.printf("Touch:%d Gear:%d Step:%d\n", touchValue, speedGear, baseStep);

  if (touchValue < touchThreshold && !touchLock) {
    touchLock = true;
    speedGear++;
    if (speedGear > 3) speedGear = 1;
  }
  if (touchValue >= touchThreshold && touchLock) {
    touchLock = false;
  }

  // 根据档位设置基础速度步长
  switch (speedGear) {
    case 1: baseStep = step1; break;
    case 2: baseStep = step2; break;
    case 3: baseStep = step3; break;
  }

  // 2. 完整呼吸渐变逻辑（修复无呼吸问题）
  ledBright = ledBright + baseStep * dir;

  // 触达亮度边界时反转渐变方向
  if (ledBright >= 255) {
    ledBright = 255;
    dir = -1; // 开始变暗
  }
  if (ledBright <= 0) {
    ledBright = 0;
    dir = 1;  // 开始变亮
  }

  analogWrite(LED_PIN, ledBright);

  delay(10);
}