// Sound Sensor with LED
// LED will turn ON when sound crosses the threshold

int soundPin = A0;    // Digital pin from sound sensor
int ledPin   = 13;   // Onboard LED

void setup() {
  pinMode(soundPin, INPUT);   // Sound sensor input
  pinMode(ledPin, OUTPUT);    // LED output
  Serial.begin(9600);
}

void loop() {
  int soundState = analogRead(soundPin);  // Read sensor
  Serial.println(soundState);              // Print for debugging

  if (soundState > 540) { 
    digitalWrite(ledPin, HIGH);  // LED ON
  } else {
    digitalWrite(ledPin, LOW);   // LED OFF
  }
  delay(200);
}
