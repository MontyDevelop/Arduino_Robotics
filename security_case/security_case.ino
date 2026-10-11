#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lockServo;

#define SERVO_PIN 9
#define BUZZER 8

String password = "1234";
String enteredPassword = "";

bool unlocked = false;

// Buzzer functions
void beep(int frequency, int duration) {
  tone(BUZZER, frequency, duration);
  delay(duration + 50);
  noTone(BUZZER);
}

void correctBeep() {
  beep(2000, 150);
  beep(2000, 150);
}

void wrongBeep() {
  beep(500, 150);
  beep(500, 150);
  beep(500, 150);
}

void lockBeep() {
  beep(1000, 500);
}

void resetBeep() {
  beep(1500, 100);
}

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lockServo.attach(SERVO_PIN);

  // Start locked
  lockServo.write(0);

  lcd.setCursor(0, 0);
  lcd.print("PASSWORD LOCK");

  delay(1500);
  showPasswordScreen();

  Serial.println("==============================");
  Serial.println("PASSWORD LOCK SYSTEM");
  Serial.println("==============================");
  Serial.println("Enter password and press ENTER");
  Serial.println("R = Reset password");
  Serial.println("L = Lock");
}

void loop() {
  if (Serial.available()) {
    char key = Serial.read();

    if (key == '\n' || key == '\r') {
      if (enteredPassword.length() > 0) {
        checkPassword();
      }
      return;
    }

    if (key == 'R' || key == 'r') {
      resetPassword();
      return;
    }

    if (key == 'L' || key == 'l') {
      lockSystem();
      return;
    }

    enteredPassword += key;

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);

    int stars = enteredPassword.length();
    if (stars > 16) stars = 16;

    for (int i = 0; i < stars; i++) {
      lcd.print("*");
    }
  }
}

void checkPassword() {
  if (enteredPassword == password) {
    unlocked = true;

    correctBeep();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS GRANTED");
    lcd.setCursor(0, 1);
    lcd.print("Door Unlocked");

    Serial.println("ACCESS GRANTED!");
    Serial.println("Door Unlocked.");

    lockServo.write(90);
    delay(3000);

  } else {
    wrongBeep();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WRONG PASSWORD");
    lcd.setCursor(0, 1);
    lcd.print("Try Again");

    Serial.println("WRONG PASSWORD!");

    delay(2000);
  }

  enteredPassword = "";

  if (unlocked) {
    showUnlockedScreen();
  } else {
    showPasswordScreen();
  }
}

void showPasswordScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}

void showUnlockedScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door Unlocked");
  lcd.setCursor(0, 1);
  lcd.print("Press L to Lock");
}

void lockSystem() {
  lockServo.write(0);

  unlocked = false;
  enteredPassword = "";

  lockBeep();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOOR LOCKED");

  Serial.println("Door Locked.");

  delay(1500);
  showPasswordScreen();
}

void resetPassword() {
  enteredPassword = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RESET PASSWORD");

  Serial.println();
  Serial.println("RESET PASSWORD");
  Serial.println("Type new password");
  Serial.println("and press ENTER:");

  resetBeep();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New Password:");

  String newPassword = "";

  while (true) {
    if (Serial.available()) {
      char key = Serial.read();

      if (key == '\n' || key == '\r') {
        if (newPassword.length() > 0) {
          password = newPassword;

          Serial.println("Password changed!");

          correctBeep();

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PASSWORD");
          lcd.setCursor(0, 1);
          lcd.print("CHANGED!");

          delay(2000);
          showPasswordScreen();
          return;
        }
      } else {
        newPassword += key;

        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);

        int stars = newPassword.length();
        if (stars > 16) stars = 16;

        for (int i = 0; i < stars; i++) {
          lcd.print("*");
        }
      }
    }
  }
}
