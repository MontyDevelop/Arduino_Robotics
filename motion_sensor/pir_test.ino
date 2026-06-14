#define PIR_PIN 2

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);

  Serial.println("PIR Sensor Testing...");
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion Detected!");
  } else {
    Serial.println("No Motion");
  }

  delay(500);
}