#define PIR 2
#define LED 7

void setup() {
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int motion = digitalRead(PIR);

  if (motion == HIGH) {
    digitalWrite(LED, HIGH);
    Serial.println("Motion Detected!");
  } 
  else {
    digitalWrite(LED, LOW);
    Serial.println("No Motion");
  }

  delay(200);
}