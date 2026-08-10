/*
  LASER DATA TRANSFER — RECEIVER
  ESP8266 (NodeMCU / Wemos D1 Mini) + LDR + I2C 16x2 LCD

  Reads light pulses through the LDR, decodes them back into text,
  and shows the result on the LCD. BIT_DURATION must match the
  transmitter sketch exactly.

  Wiring:
    LDR:  3.3V -- LDR -- A0 -- 10k resistor -- GND
    LCD:  SDA -> D2, SCL -> D1, VCC -> 3.3V/5V (check your board), GND -> GND

  Library needed: "LiquidCrystal I2C" (Arduino Library Manager —
  the version by Frank de Brabander works well). If your library
  version doesn't have lcd.init(), try lcd.begin(16, 2) instead.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LDR_PIN      A0
#define BIT_DURATION 100     // ms per bit -- MUST match the transmitter sketch

// If the screen stays blank/garbled, try address 0x3F instead of 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte SYNC_BYTE = 0b10101010;
int threshold = 889;         // replaced by calibrateThreshold() at startup

void calibrateThreshold() {
  Serial.println("=== Calibration ===");
  Serial.println("Keep the laser OFF (not pointed at the LDR).");
  Serial.println("Press Enter when ready...");
  while (!Serial.available()) delay(10);
  while (Serial.available()) Serial.read();

  long darkSum = 0;
  for (int i = 0; i < 50; i++) { darkSum += analogRead(LDR_PIN); delay(20); }
  int darkAvg = darkSum / 50;
  Serial.print("Dark level: ");
  Serial.println(darkAvg);

  Serial.println("Now turn the laser ON, aimed steadily at the LDR.");
  Serial.println("Press Enter when ready...");
  while (!Serial.available()) delay(10);
  while (Serial.available()) Serial.read();

  long lightSum = 0;
  for (int i = 0; i < 50; i++) { lightSum += analogRead(LDR_PIN); delay(20); }
  int lightAvg = lightSum / 50;
  Serial.print("Light level: ");
  Serial.println(lightAvg);

  threshold = (darkAvg + lightAvg) / 2;
  Serial.print("Threshold set to: ");
  Serial.println(threshold);

  if (lightAvg - darkAvg < 50) {
    Serial.println("WARNING: dark/light levels are too close together.");
    Serial.println("Improve alignment, dim the room, or shield the LDR from ambient light.");
  }

  lcd.clear();
  lcd.print("Calibrated! Th=");
  lcd.setCursor(0, 1);
  lcd.print(threshold);
  delay(1500);
}

bool readBit() {
  return analogRead(LDR_PIN) > threshold;
}

byte receiveByte() {
  byte b = 0;
  for (int i = 0; i < 8; i++) {
    delay(BIT_DURATION);
    b = (b << 1) | readBit();
  }
  return b;
}

void showMessage(String msg, bool valid) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg.substring(0, min((int)msg.length(), 16)));
  lcd.setCursor(0, 1);
  if (msg.length() > 16) {
    lcd.print(msg.substring(16, min((int)msg.length(), 32)));
  } else {
    lcd.print(valid ? "(OK)" : "(checksum err)");
  }
}

void receiveMessage() {
  // Busy-wait for the laser to switch ON -- this rising edge is the
  // first '1' bit of SYNC_BYTE, and it's our signal that a message is starting.
  while (analogRead(LDR_PIN) < threshold) {
    delay(1);
  }
  delay(BIT_DURATION / 2);     // center our sampling inside that first bit

  byte sync = 1;                // the bit that triggered us was a '1'
  for (int i = 0; i < 7; i++) {
    delay(BIT_DURATION);
    sync = (sync << 1) | readBit();
  }
  if (sync != SYNC_BYTE) return;   // false trigger (noise/flicker) -- ignore and retry

  byte len = receiveByte();
  if (len == 0 || len > 64) return;   // implausible -- likely a misread, retry

  String msg = "";
  byte checksum = 0;
  for (int i = 0; i < len; i++) {
    byte c = receiveByte();
    msg += (char)c;
    checksum ^= c;
  }

  byte recvChecksum = receiveByte();
  bool valid = (recvChecksum == checksum);

  Serial.print(valid ? "Received OK: " : "CHECKSUM MISMATCH: ");
  Serial.println(msg);

  showMessage(msg, valid);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();          // D2 = SDA, D1 = SCL by default on NodeMCU
  lcd.init();
  lcd.backlight();
  lcd.print("Laser RX ready");

  calibrateThreshold();

  lcd.clear();
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("transmission...");
}

void loop() {
  receiveMessage();
}
