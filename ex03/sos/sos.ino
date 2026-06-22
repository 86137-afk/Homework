const int ledPin = 2;
unsigned long prevTime = 0;
unsigned long now;
int state = 0;
// 定时参数
const unsigned long dot = 200;    // 短闪时长
const unsigned long dash = 600;   // 长闪时长
const unsigned long gap = 200;    // 字符间隔
const unsigned long letterGap = 500;
const unsigned long wordGap = 2000;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  now = millis();
  switch(state){
    // S 3短闪
    case 0: digitalWrite(ledPin,HIGH); prevTime=now; state++; break;
    case 1: if(now-prevTime>=dot){digitalWrite(ledPin,LOW);prevTime=now;state++;} break;
    case 2: if(now-prevTime>=gap){state=0;static int cnt=0;cnt++;if(cnt>=3){cnt=0;state=3;}} break;
    // O 3长闪
    case 3: digitalWrite(ledPin,HIGH); prevTime=now; state++; break;
    case 4: if(now-prevTime>=dash){digitalWrite(ledPin,LOW);prevTime=now;state++;} break;
    case 5: if(now-prevTime>=gap){state=3;static int cnt=0;cnt++;if(cnt>=3){cnt=0;state=6;}} break;
    // S 3短闪
    case 6: digitalWrite(ledPin,HIGH); prevTime=now; state++; break;
    case 7: if(now-prevTime>=dot){digitalWrite(ledPin,LOW);prevTime=now;state++;} break;
    case 8: if(now-prevTime>=gap){state=6;static int cnt=0;cnt++;if(cnt>=3){cnt=0;state=9;}} break;
    // 一组SOS结束等待
    case 9: if(now-prevTime>=wordGap){state=0;} break;
  }
}