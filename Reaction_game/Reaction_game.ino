// --- Pins ---
const int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};     
const int segPins[7] = {30, 31, 32, 33, 34, 35, 36};         
const int buttonPin = 10;                                    

// --- Score and Level ---
int score = 0;
int level = 1;
int pointsToNextLevel = 5;

// --- Reaction Times per Level ---
int levelTime[] = {500, 400, 300, 200, 100};

// --- 7-segment digit patterns (a-g)
const bool digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup() {
  // Setup pins
  for (int i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // button with internal pull-up

  displayNumber(score); // show 0 at start
  randomSeed(analogRead(A0)); // random setup
}

void loop() {
  // --- Button Hold to Show Level ---
  if (digitalRead(buttonPin) == LOW) {
    unsigned long pressedTime = millis();
    while (digitalRead(buttonPin) == LOW) {
      if (millis() - pressedTime > 2000) {
        showLevelTemporarily();
        break;
      }
    }
  }

  // --- Main Game Logic ---
  int target = random(0, 8);
  digitalWrite(ledPins[target], HIGH);

  bool success = false;
  unsigned long start = millis();
  while (millis() - start < levelTime[level - 1]) {
    if (digitalRead(buttonPin) == LOW) {
      success = true;
      break;
    }
  }

  digitalWrite(ledPins[target], LOW);

  if (success) {
    score++;
    flashLED(target);
    if (score % pointsToNextLevel == 0 && level < 5) {
      level++;
      levelFlash(level);
    }
  } else {
    score = 0;
    level = 1;
    failFlash();
  }

  displayNumber(score);
  delay(500);
}

// Display number 0-9 on 7-segment
void displayNumber(int num) {
  if (num < 0 || num > 9) return;
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digits[num][i] ? HIGH : LOW);
  }
}

// Flash success LED quickly
void flashLED(int pinIndex) {
  digitalWrite(ledPins[pinIndex], LOW);
  delay(100);
  digitalWrite(ledPins[pinIndex], HIGH);
  delay(100);
  digitalWrite(ledPins[pinIndex], LOW);
}

// Flash all LEDs on level up
void levelFlash(int lvl) {
  for (int i = 0; i < lvl; i++) {
    for (int j = 0; j < 8; j++) digitalWrite(ledPins[j], HIGH);
    delay(150);
    for (int j = 0; j < 8; j++) digitalWrite(ledPins[j], LOW);
    delay(150);
  }
}

// Blink all LEDs on fail
void failFlash() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 8; j++) digitalWrite(ledPins[j], HIGH);
    delay(200);
    for (int j = 0; j < 8; j++) digitalWrite(ledPins[j], LOW);
    delay(200);
  }
}

// Show current level on 7-segment for 2 seconds
void showLevelTemporarily() {
  displayNumber(level);   // Show level
  delay(2000);            // Wait 2 seconds
  displayNumber(score);   // Go back to score
}
