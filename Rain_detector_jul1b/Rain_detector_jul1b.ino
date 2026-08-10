#define RAIN_SENSOR A0
#define LED 8
#define BUZZER 9

int rainValue;

void setup() {

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  Serial.println("Rain Detection System");
}

void loop() {

  rainValue = analogRead(RAIN_SENSOR);

  Serial.print("Rain Value : ");
  Serial.println(rainValue);

  // 🌞 No Rain
  if (rainValue > 850) {

    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);

    Serial.println("No Rain");
  }

  // 🌦️ Light Rain
  else if (rainValue > 500 && rainValue <= 850) {

    digitalWrite(LED, HIGH);

    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(300);

    Serial.println("Light Rain Detected");
  }

  // 🌧️ Heavy Rain
  else {

    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);

    Serial.println("Heavy Rain Detected");

    delay(200);
  }
}