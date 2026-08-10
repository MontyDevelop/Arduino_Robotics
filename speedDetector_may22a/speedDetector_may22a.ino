#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trig1 2
#define echo1 3

#define trig2 4
#define echo2 5

#define buzzer 8
#define led 9

long duration;
float distance;

unsigned long startTime;
unsigned long endTime;

bool firstDetected = false;

// Distance between sensors in cm
float sensorDistance = 20.0;

// Speed limit in km/h
float speedLimit = 50.0;

void setup() {

  Serial.begin(9600);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);

  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Speed Detector");
  delay(2000);
  lcd.clear();
}

float getDistance(int trig, int echo) {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);

  distance = duration * 0.034 / 2;

  return distance;
}

void loop() {

  float d1 = getDistance(trig1, echo1);
  float d2 = getDistance(trig2, echo2);

  // First sensor detect
  if (d1 < 10 && !firstDetected) {

    startTime = millis();
    firstDetected = true;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vehicle Found");
  }

  // Second sensor detect/
  if (d2 < 10 && firstDetected) {

    endTime = millis();

    float timeTaken = (endTime - startTime) / 1000.0;

    // Speed in cm/s
    float speedCms = sensorDistance / timeTaken;

    // Convert to km/h
    float speedKmh = speedCms * 0.036;

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Speed:");
    lcd.print(speedKmh);
    lcd.print("km/h");

    // Overspeed Alert
    if (speedKmh > speedLimit) {

      lcd.setCursor(0,1);
      lcd.print("OVER SPEED!");

      digitalWrite(buzzer, HIGH);
      digitalWrite(led, HIGH);

      delay(2000);

      digitalWrite(buzzer, LOW);
      digitalWrite(led, LOW);

    } else {

      lcd.setCursor(0,1);
      lcd.print("Normal Speed");
    }

    firstDetected = false;

    delay(3000);
  }
}