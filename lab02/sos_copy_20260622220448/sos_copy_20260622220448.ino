// 定义LED引脚，ESP32板载LED在GPIO2
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // ===== S：短闪3次 =====
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500); // S与O之间字母间隔

  // ===== O：长闪3次 =====
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(600);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500); // O与S之间字母间隔

  // ===== S：短闪3次 =====
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(2000); // 一组SOS完成，长时间间隔后重复
}