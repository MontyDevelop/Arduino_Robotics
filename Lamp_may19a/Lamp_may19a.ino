#define LDR 2
#define LED 8

int lightValue;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  Serial.begin(9600);
}

void loop() {
  lightValue = digitalRead(LDR);

  Serial.print("Light: ");
  Serial.println(lightValue);

  if (lightValue == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  delay(200);
}