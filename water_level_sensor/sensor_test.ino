#define WATER_SENSOR A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int waterLevel = analogRead(WATER_SENSOR);

  Serial.print("Water Level: ");
  Serial.println(waterLevel);

  delay(500);
}