#define FLAME_SENSOR A0
#define BUZZER 8

#define THRESHOLD 700    // Adjust according to your sensor

void setup() {

  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int flameValue = analogRead(FLAME_SENSOR);

  Serial.print("Flame Value: ");
  Serial.println(flameValue);

  if (flameValue < THRESHOLD) {

    Serial.println("FLAME DETECTED!");

    digitalWrite(BUZZER, HIGH);
  }
  else {

    Serial.println("No Flame");

    digitalWrite(BUZZER, LOW);
  }

  delay(200);
}