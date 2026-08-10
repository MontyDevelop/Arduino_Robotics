#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_LED     8
#define YELLOW_LED  9
#define GREEN_LED   10

#define P1_BUTTON   2
#define P2_BUTTON   3

unsigned long startTime;

void setup() {

  lcd.init();
  lcd.backlight();

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(P1_BUTTON, INPUT_PULLUP);
  pinMode(P2_BUTTON, INPUT_PULLUP);

  randomSeed(analogRead(A0));
}

void loop() {

  // -------- STOP --------
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("STOP");
  lcd.setCursor(3, 1);
  lcd.print("WAIT");

  delay(2000);

  // -------- GET READY --------
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GET READY");
  lcd.setCursor(0, 1);
  lcd.print("Don't Press");

  delay(random(2000, 5000));

  // -------- GO --------
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("GO!");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");

  startTime = millis();

  while (true) {

    if (digitalRead(P1_BUTTON) == LOW) {

      unsigned long reaction = millis() - startTime;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Winner: Player1");

      lcd.setCursor(0, 1);
      lcd.print("Time:");
      lcd.print(reaction);
      lcd.print("ms");

      break;
    }

    if (digitalRead(P2_BUTTON) == LOW) {

      unsigned long reaction = millis() - startTime;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Winner: Player2");

      lcd.setCursor(0, 1);
      lcd.print("Time:");
      lcd.print(reaction);
      lcd.print("ms");

      break;
    }
  }

  delay(5000);

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}