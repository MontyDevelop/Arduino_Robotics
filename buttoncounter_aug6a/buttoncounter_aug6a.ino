#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int countButton = 2;
const int resetButton = 3;

int count = 0;

bool lastCountState = HIGH;
bool lastResetState = HIGH;

void setup()
{
  pinMode(countButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Button Counter");

  lcd.setCursor(0, 1);
  lcd.print("Count : 0");
}

void loop()
{
  bool currentCountState = digitalRead(countButton);
  bool currentResetState = digitalRead(resetButton);

  // Count Button
  if (lastCountState == HIGH && currentCountState == LOW)
  {
    count++;

    updateDisplay();

    delay(200);
  }

  // Reset Button
  if (lastResetState == HIGH && currentResetState == LOW)
  {
    count = 0;

    updateDisplay();

    delay(200);
  }

  lastCountState = currentCountState;
  lastResetState = currentResetState;
}

void updateDisplay()
{
  lcd.setCursor(0, 1);
  lcd.print("Count :      ");   // Purani value clear

  lcd.setCursor(8, 1);
  lcd.print(count);
}