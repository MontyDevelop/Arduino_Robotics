
int sensor = 2;
int buzzer = 8;

void setup() {
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int val = digitalRead(sensor);

  if (val == LOW) {
    // Doorbell sound
    tone(buzzer, 1000);
    delay(200);

    tone(buzzer, 1500);
    delay(200);

    noTone(buzzer);
    delay(1000); // avoid continuous ringing
  }
}