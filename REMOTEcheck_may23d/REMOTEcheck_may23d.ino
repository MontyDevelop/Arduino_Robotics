#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2

void setup() {

  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR Receiver Ready");
}

void loop() {

  if (IrReceiver.decode()) {

    Serial.print("Code: ");

    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    IrReceiver.resume();
  }
}