// 🎂 Happy Birthday Song using Buzzer 🎵

int buzzerPin = 8;

// Notes
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  // 🎵 Happy Birthday to You
  tone(buzzerPin, C); delay(300);
  tone(buzzerPin, C); delay(300);
  tone(buzzerPin, D); delay(600);
  tone(buzzerPin, C); delay(600);
  tone(buzzerPin, F); delay(600);
  tone(buzzerPin, E); delay(900);

  // 🎵 Happy Birthday to You
  tone(buzzerPin, C); delay(300);
  tone(buzzerPin, C); delay(300);
  tone(buzzerPin, D); delay(600);
  tone(buzzerPin, C); delay(600);
  tone(buzzerPin, G); delay(600);
  tone(buzzerPin, F); delay(900);

  noTone(buzzerPin);
  delay(2000);
}