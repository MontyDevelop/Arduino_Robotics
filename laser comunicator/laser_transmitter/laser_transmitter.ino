/*
  LASER DATA TRANSFER — TRANSMITTER
  ESP8266 (NodeMCU / Wemos D1 Mini) + Laser diode module

  HOW TO USE:
  Open the Serial Monitor at 115200 baud, type a message, press Enter.
  The message is sent as laser ON/OFF pulses (ON = 1, OFF = 0).

  IMPORTANT: BIT_DURATION must be IDENTICAL on the transmitter and the
  receiver sketch. An LDR is a slow sensor, so start slow (100ms/bit)
  and only speed up once everything works reliably.
*/

#define LASER_PIN    D1     // GPIO5 -> laser module's signal/control pin
#define BIT_DURATION 100    // ms per bit -- MUST match the receiver sketch

const byte SYNC_BYTE = 0b10101010;   // lets the receiver find the start of a message

void setup() {
  Serial.begin(115200);
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);      // idle state = laser off

  Serial.println();
  Serial.println("=== Laser transmitter ready ===");
  Serial.println("Type a message and press Enter to send it.");
}

void sendBit(bool bit) {
  digitalWrite(LASER_PIN, bit ? HIGH : LOW);
  delay(BIT_DURATION);
}

void sendByte(byte b) {
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i) & 0x01);        // send MSB first
  }
}

void sendMessage(String msg) {
  Serial.print("Sending: ");
  Serial.println(msg);

  digitalWrite(LASER_PIN, LOW);
  delay(500);                        // guaranteed idle gap before a new message

  sendByte(SYNC_BYTE);               // sync pattern
  sendByte((byte)msg.length());      // how many characters follow

  byte checksum = 0;
  for (unsigned int i = 0; i < msg.length(); i++) {
    byte c = (byte)msg[i];
    sendByte(c);
    checksum ^= c;                   // simple XOR checksum for error detection
  }
  sendByte(checksum);

  digitalWrite(LASER_PIN, LOW);      // back to idle
  Serial.println("Done.");
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    if (msg.length() > 0 && msg.length() <= 64) {
      sendMessage(msg);
    } else if (msg.length() > 64) {
      Serial.println("Message too long (max 64 characters).");
    }
  }
}
