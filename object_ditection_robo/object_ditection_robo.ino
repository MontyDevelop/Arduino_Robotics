#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG 9
#define ECHO 10

#define GREEN_LED 6
#define RED_LED 8

LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
float distance;

void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Object Detector");
  delay(2000);
  lcd.clear();
}

void loop()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);

  distance = duration * 0.0343 / 2;

  lcd.clear();

  if(distance <= 20)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);

    lcd.setCursor(0,0);
    lcd.print("Object Detect");

    lcd.setCursor(0,1);
    lcd.print("Dist:");
    lcd.print(distance,1);
    lcd.print(" cm");
  }
  else
  {

    lcd.setCursor(0,0);
    lcd.print("No Object");

    lcd.setCursor(0,1);
    lcd.print("Dist:");
    lcd.print(distance,1);
    lcd.print(" cm");
  }

  delay(300);
}