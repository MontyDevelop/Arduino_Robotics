#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lockServo;

#define SERVO_PIN 9

String password = "1234";
String enteredPassword = "";

bool unlocked = false;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lockServo.attach(SERVO_PIN);

  // Start in locked position
  lockServo.write(0);

  lcd.setCursor(0, 0);
  lcd.print("PASSWORD LOCK");

  delay(1500);

  showPasswordScreen();

  Serial.println("================================");
  Serial.println("PASSWORD LOCK SYSTEM");
  Serial.println("================================");
  Serial.println("Enter password and press ENTER");
  Serial.println("R = Reset password");
  Serial.println("L = Lock");
  Serial.println("--------------------------------");
}

void loop() {

  if (Serial.available()) {

    char key = Serial.read();

    // Ignore newline and carriage return temporarily
    if (key == '\n' || key == '\r') {

      if (enteredPassword.length() > 0) {
        checkPassword();
      }

      return;
    }

    // Reset password
    if (key == 'R' || key == 'r') {
      resetPassword();
      return;
    }

    // Lock system
    if (key == 'L' || key == 'l') {
      lockSystem();
      return;
    }

    // Add typed character to password
    enteredPassword += key;

    // Show * instead of actual password
    lcd.setCursor(0, 1);

    for (int i = 0; i < enteredPassword.length(); i++) {
      lcd.print("*");
    }
  }
}


void checkPassword() {

  if (enteredPassword == password) {

    unlocked = true;

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

  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New Password:");

  // Wait for new password
  String newPassword = "";

  while (true) {

    if (Serial.available()) {

      char key = Serial.read();

      if (key == '\n' || key == '\r') {

        if (newPassword.length() > 0) {
          password = newPassword;

          Serial.println("Password changed!");
          Serial.println("New password saved.");

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PASSWORD");
          lcd.setCursor(0, 1);
          lcd.print("CHANGED!");

          delay(2000);

          showPasswordScreen();

          return;
        }
      }

      else {
        newPassword += key;

        lcd.setCursor(0, 1);

        for (int i = 0; i < newPassword.length(); i++) {
          lcd.print("*");
        }
      }
    }
  }
}
