// LED pins from 2 to 11
int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int ledCount = 10; // Total LEDs

void setup() {
  // Set all pins as OUTPUT
  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  pattern1(); // Left to Right
  delay(500);
  pattern2(); // Right to Left
  delay(500);
  pattern3(); // Bounce effect
  delay(500);
  pattern4(); // Alternate LEDs
  delay(500);
}

// Pattern 1: Left to Right
void pattern1() {
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(leds[i], HIGH);
    delay(100);
    digitalWrite(leds[i], LOW);
  }
}

// Pattern 2: Right to Left
void pattern2() {
  for (int i = ledCount - 1; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(100);
    digitalWrite(leds[i], LOW);
  }
}

// Pattern 3: Bounce Effect
void pattern3() {
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(leds[i], HIGH);
    delay(50);
    digitalWrite(leds[i], LOW);
  }
  for (int i = ledCount - 2; i > 0; i--) { // Reverse without repeating ends
    digitalWrite(leds[i], HIGH);
    delay(50);
    digitalWrite(leds[i], LOW);
  }
}

// Pattern 4: Alternate LEDs
void pattern4() {
  for (int i = 0; i < ledCount; i += 2) {
    digitalWrite(leds[i], HIGH);
  }
  delay(300);
  for (int i = 0; i < ledCount; i += 2) {
    digitalWrite(leds[i], LOW);
  }

  for (int i = 1; i < ledCount; i += 2) {
    digitalWrite(leds[i], HIGH);
  }
  delay(300);
  for (int i = 1; i < ledCount; i += 2) {
    digitalWrite(leds[i], LOW);
  }
}
