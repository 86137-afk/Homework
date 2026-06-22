// ex02 millis实现1Hz LED闪烁（1秒周期）
const int ledPin = 2;
unsigned long prevTime = 0;
const unsigned long interval = 1000; // 1000ms = 1Hz
bool ledStatus = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  if(now - prevTime >= interval){
    prevTime = now;
    ledStatus = !ledStatus;
    digitalWrite(ledPin, ledStatus);
  }
}