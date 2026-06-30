// 定义两路LED引脚
const int led1Pin = 2;  
const int led2Pin = 4;  

// PWM公共参数
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);
  // 两路引脚分别绑定PWM通道
  ledcAttach(led1Pin, freq, resolution);
  ledcAttach(led2Pin, freq, resolution);
}

// 封装单次完整呼吸函数
void breatheLED(int pin, int delayTime)
{
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(pin, dutyCycle);   
    delay(delayTime);
  }
  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(pin, dutyCycle);   
    delay(delayTime);
  }
}

void loop() {
  int val;
  // LED1渐亮，LED2渐暗
  for(val = 0; val <= 255; val++)
  {
    ledcWrite(led1Pin, val);
    ledcWrite(led2Pin, 255 - val);
    delay(10);
  }
  // LED1渐暗，LED2渐亮
  for(val = 255; val >= 0; val--)
  {
    ledcWrite(led1Pin, val);
    ledcWrite(led2Pin, 255 - val);
    delay(10);
  }
  Serial.println("交替一轮完成");
}