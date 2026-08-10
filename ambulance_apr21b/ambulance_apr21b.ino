int red = 9;
int blue = 10;
int buzzer = 8;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  // 🔴 RED phase
  digitalWrite(red, HIGH);
  digitalWrite(blue, LOW);

  // rising siren
  for (int f = 700; f <= 1200; f += 20) {
    tone(buzzer, f);
    delay(10);
  }

  // 🔵 BLUE phase
  digitalWrite(red, LOW);
  digitalWrite(blue, HIGH);

  // falling siren
  for (int f = 1200; f >= 700; f -= 20) {
    tone(buzzer, f);
    delay(10);
  }
}