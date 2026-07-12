#include <LiquidCrystal.h> // here we are including the lib nedeed to use lcd isplay
#define PIR 6  // here PIR is in captal letter to avoid the clash between variable and pin name
#define LED 31

// pin selections for the lcd circuit. 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Here const means that once you assign the pins to the lcd later 
// you do not want and able to change them until you can make the changes here....
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  lcd.begin(16, 2);  // Here (16,2) is (max_number_of_column, max_number_of_row)
  // lcd.print("hello, world!"); // you can possibly avoid this line to be here and can  be used inside the loop as well.
}

void loop() {
  int pir_data = digitalRead(PIR); // here pir_data is in lower letter to avoid the clash between variable and pin name
  lcd.setCursor(0, 0); // Here (0,1) indicates (column_number, row_number)
  // lcd.println(pir_data); // ln with print is use to print the data in new line when your data comes continiously
  if(pir_data == 1) {
    digitalWrite(LED, HIGH); 
    lcd.println("Motion detected!");
  }
  else {
    digitalWrite(LED, LOW);
    lcd.println("No Motion!       ");
  }
}

