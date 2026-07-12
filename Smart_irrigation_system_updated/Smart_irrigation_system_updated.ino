#include <LiquidCrystal.h>

// LCD pin setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int soilPin = A0;      // Soil sensor analog output
const int relayPin = 7;      // Relay control pin
int soilValue = 0;           // Variable to store soil reading

void setup() {
  lcd.begin(16, 2);            // Initialize 16x2 LCD
  pinMode(relayPin, OUTPUT);   // Set relay pin as output
  digitalWrite(relayPin, HIGH); // Keep pump OFF initially

  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  delay(2000);                 // Show welcome screen
  lcd.clear();
}

void loop() {
  soilValue = analogRead(soilPin);  // Read moisture level
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(soilValue);

  // Moisture levels: 0 = wet, ~1023 = dry
  if (soilValue > 600) {
    // Soil is dry, start watering
    digitalWrite(relayPin, HIGH); // Relay ON (pump ON)
    lcd.setCursor(0, 1);
    lcd.print("WATERING...     ");
  } else {
    // Soil is wet, stop watering
    digitalWrite(relayPin, LOW); // Relay OFF (pump OFF)
    lcd.setCursor(0, 1);
    lcd.print("Soil is OK :)   ");
  }

  delay(2000); // Wait before next reading
}

