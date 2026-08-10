#define GAS_SENSOR A0
#define RED_LED 8
#define BUZZER 9

#define THRESHOLD 400

int gasValue;

void setup() {

  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  Serial.println("Gas Leak Alert System");
}

void loop() {

  gasValue = analogRead(GAS_SENSOR);

  Serial.print("Gas Value : ");
  Serial.println(gasValue);

  if (gasValue > THRESHOLD) {

    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);

    Serial.println("⚠ GAS LEAK DETECTED!");

  }
  else {

    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    Serial.println("AIR SAFE");
  }

  delay(200);
}