// Automatic Light Lamp using LDR Module 🌙💡

int ldrPin = A0;
int lampPin = 13;

void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(lampPin, OUTPUT);
}

void loop() {

  int lightLevel = analogRead(ldrPin);

  if (lightLevel < 500) {
    // 🌙 It is dark → Turn lamp ON
    digitalWrite(lampPin, HIGH);
  }
  else {
    // ☀️ It is bright → Turn lamp OFF
    digitalWrite(lampPin, LOW);
  }
}