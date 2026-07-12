#include <Keypad.h>
#include <LiquidCrystal.h>


const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9,8,7,6}; 
byte colPins[COLS] = {5,4,3,2}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


const int rs = 13, en = 12, d4 = 30, d5 = 31, d6 = 32, d7 = 33;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  Serial.println("4x4 Keypad Test");
  lcd.begin(16, 2);
}

void loop() {
  lcd.setCursor(0, 0);

  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: ");
    lcd.print("Key Pressed: ");
    lcd.setCursor(13,0);
    Serial.println(key);
    lcd.print(key);
  }
}