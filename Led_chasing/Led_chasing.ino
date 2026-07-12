// Define LED pins from 2 to 11 (10 LEDs)
int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int numLeds = sizeof(leds) / sizeof(leds[0]);

void setup() {
  // Set all LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // Chasing: Left to Right
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);  // Turn on current LED
    delay(100);                   // Hold for 100 ms
    digitalWrite(leds[i], LOW);   // Turn it off
  }
}
