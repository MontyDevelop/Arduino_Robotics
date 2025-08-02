// Simple PIR Motion Detector
// PIR output -> pin 7
// Optional: onboard LED (pin 13) lights when motion is detected

const int pirPin = 7;        // PIR sensor output
const int ledPin = 13;       // Built-in LED

void setup() {
  pinMode(pirPin, INPUT);    // PIR is input
  pinMode(ledPin, OUTPUT);   // LED is output
  Serial.begin(9600);        // For debugging / viewing motion events
}

void loop() {
  int motion = digitalRead(pirPin);  // Read PIR output

  if (motion == HIGH) {
    // Motion detected
    digitalWrite(ledPin, HIGH);      // Turn LED on
    Serial.println("Motion detected!");
  } else {
    // No motion
    digitalWrite(ledPin, LOW);       // Turn LED off
    Serial.println("No motion");
  }

  delay(500); // Check twice per second
}
