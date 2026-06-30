#define LED_A 2
#define LED_B 4

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    analogWrite(LED_A, i);          // A 慢慢变亮
    analogWrite(LED_B, 255 - i);    // B 同时慢慢变暗
    delay(20);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(LED_A, i);          // A 慢慢变暗
    analogWrite(LED_B, 255 - i);    // B 同时慢慢变亮
    delay(20);
  }
}