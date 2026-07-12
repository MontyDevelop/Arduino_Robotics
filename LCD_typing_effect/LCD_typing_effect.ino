#include <LiquidCrystal.h>

// RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String messages[] = {
  "STEAMX Academy",
  "Robotics",
  "Arduino Projects",
  "Python Coding",
  "AI & ML",
  "Abacus",
  "Admissions Open"
};

const int numMessages = sizeof(messages) / sizeof(messages[0]);

void typeText(String text) {
  lcd.clear();

  // Center short text on first line
  int startPos = max(0, (16 - text.length()) / 2);

  lcd.setCursor(startPos, 0);

  for (int i = 0; i < text.length(); i++) {
    lcd.print(text[i]);
    delay(120);   // Typing speed
  }

  delay(1500);    // Hold message
}

void setup() {
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  delay(1000);
}

void loop() {
  for (int i = 0; i < numMessages; i++) {
    typeText(messages[i]);
  }
}