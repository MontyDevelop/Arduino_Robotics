#include <LedControl.h>
LedControl lc = LedControl(10, 12, 11, 1);
const int soundPin = A0; 
const int digitalPin = 2;
int noiseFloor = 0;     
int sensitivity = 10;     
int level = 0;
void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);   // brightness (0–15)
  lc.clearDisplay(0);
  pinMode(digitalPin, INPUT);
  Serial.println("Calibrating noise floor...");
  long total = 0;
  for (int i = 0; i < 200; i++) {
    total += analogRead(soundPin);
    delay(2);
  }
  noiseFloor = total / 200;
  Serial.print("Noise floor set to: ");
  Serial.println(noiseFloor);
  Serial.println("Ready!");
}
void loop() {
  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += analogRead(soundPin);
    delay(2);
  }
  int soundValue = total / 8;
  int diff = soundValue - noiseFloor;
  // 
  if (diff > sensitivity) {
    int barLevel = map(diff, sensitivity, 300, 1, 8); 
    barLevel = constrain(barLevel, 0, 8);
    lc.clearDisplay(0);
    for (int row = 0; row < barLevel; row++) {
      for (int col = 0; col < 8; col++) {
        lc.setLed(0, 7 - row, col, true);
      }
    }
    level = barLevel;
  } else {
    lc.clearDisplay(0); 
    level = 0;
  }

  Serial.print("Analog: ");
  Serial.print(soundValue);
  Serial.print(" | Diff: ");
  Serial.print(diff);
  Serial.print(" | Level: ");
  Serial.print(level);
  Serial.print(" | Digital: ");
  Serial.println(digitalRead(digitalPin));
  delay(40);
}