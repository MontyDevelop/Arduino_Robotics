#include <LiquidCrystal.h>

// Integer Bibliothek
int hintergrundLichtPIN = 8;			// Hintergrundlicht PIN
int hintergrundLichtWert = 49;    // Hintergrund Wert = Höher => Heller

int kontrastPIN = 9;							// Kontrast PIN
int kontrastWert = 49;            // Kontrast Wert = Kleiner => mehr Kontrast

int analogWert;

// Setup der LCD Bibliothek
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {

  // Computer Schnittstellen-Verbindung
  Serial.begin(9600);
  
  // Setup der PINs
  pinMode(hintergrundLichtPIN, OUTPUT);
  pinMode(kontrastPIN, OUTPUT);
  
  // Kontrast und Hintergrund Steuerung des LCD
  analogWrite(hintergrundLichtPIN, hintergrundLichtWert);
  analogWrite(kontrastPIN, kontrastWert);
  
  // Einführung des LCD in den Microkontroller
  lcd.begin(16, 2);  
  
}

void loop() {

  // Schreiben des AnalogInputs A0
  analogWert = analogRead(A0) / 100;

  // Wertausgabe in die Console(Debugging)
  Serial.print(analogWert);
  Serial.print("\n");             // Neue Zeile
  
  // Sender der Information an den LCD
  lcd.setCursor(0, 0);
  lcd.print("Potentiometer");
  lcd.setCursor(0, 1);
  lcd.print(analogWert);
  lcd.setCursor(2, 1);
	lcd.print("k");
	delay(100);
	
	// Löschung des Feldes(zweite Zeile, zweite Reihe) im LCD
	// Sonst bleibt eine 0 stehen, wenn man auf 10k geht und wieder zurück geht
	lcd.setCursor(1, 1);
	lcd.print(" ");
  delay(7);

}