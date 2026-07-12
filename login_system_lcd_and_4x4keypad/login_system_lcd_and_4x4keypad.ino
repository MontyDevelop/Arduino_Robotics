#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(13, 12, 30, 31, 32, 33);

String password = "1234";  // Set your password here
String input = "";

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
}

void loop() {
  char key = keypad.getKey();

  if (key) {


    if (key == '*') {
      input = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
    }


    else if (key == '#') {

      lcd.clear();

      if (input == password) {
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");

        Serial.println("Access Granted");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");

        Serial.println("Access Denied");
      }

      delay(2000);


      input = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
    }


    else {
      if (input.length() < 8) {
        input += key;

        lcd.print("*");
      }
    }
  }
}