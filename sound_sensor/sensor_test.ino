#define SOUND_PIN A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int soundValue = analogRead(SOUND_PIN);

  Serial.print("Sound Level: ");
  Serial.println(soundValue);

  delay(100);
}