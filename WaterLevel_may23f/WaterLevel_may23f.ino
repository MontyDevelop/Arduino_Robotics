#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int waterSensor = A0;

int waterValue = 0;

void setup() {

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Water Monitor");

  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {

  waterValue = analogRead(waterSensor);

  Serial.println(waterValue);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Water Level:");

  if (waterValue < 200) {

    lcd.setCursor(0,1);
    lcd.print("EMPTY");

  }

  else if (waterValue >= 200 && waterValue < 400) {

    lcd.setCursor(0,1);
    lcd.print("Medium");

  }

  else {

    lcd.setCursor(0,1);
    lcd.print("FULL");

  }

  delay(1000);
}