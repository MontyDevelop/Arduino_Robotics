// Pulse Sensor connected to A0, LED on pin 13
const int pulsePin = A0;  // Signal pin
const int ledPin = 13;    // Optional heartbeat LED
int signalValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  // Start serial monitor
}

void loop() {
  signalValue = analogRead(pulsePin);  // Read pulse sensor
  Serial.println(signalValue);         // Output to Serial Monitor

  // Optional: Blink LED when pulse is detected
  if (signalValue > 400) { // Threshold might vary
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(100); // Small delay for stability
}
