#define HALL_SENSOR 2
#define RED_LED 8
#define BUZZER 9

void setup() {

  pinMode(HALL_SENSOR, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int sensorValue = digitalRead(HALL_SENSOR);

  // Most Hall Effect modules become LOW when magnet is detected
  if (sensorValue == LOW) {

    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 1000);

    Serial.println("MAGNET DETECTED!");

  }
  else {

    digitalWrite(RED_LED, LOW);

    noTone(BUZZER);

    Serial.println("NO MAGNET");
  }

  delay(200);
}