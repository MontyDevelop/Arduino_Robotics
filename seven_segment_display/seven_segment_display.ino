// 8-LED Chase Effect for Arduino Mega
// LEDs connected to digital pins 22..29 with 220Ω resistors to GND

const int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  // Set all LED pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // start turned off
  }
}

void loop() {
  // Light them one by one forward
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], HIGH); // turn on
    delay(150);                     // small pause so kids can see
    digitalWrite(ledPins[i], LOW);  // turn off
  }

  // Then backwards
  for (int i = 7; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(150);
    digitalWrite(ledPins[i], LOW);
  }
}
