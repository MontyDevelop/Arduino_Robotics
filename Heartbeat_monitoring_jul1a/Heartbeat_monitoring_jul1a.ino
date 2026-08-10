#include <PulseSensorPlayground.h>

#define PULSE_PIN A0

PulseSensorPlayground pulseSensor;

void setup() {

  Serial.begin(9600);

  pulseSensor.analogInput(PULSE_PIN);

  pulseSensor.setThreshold(550);

  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Ready");
  }
}

void loop() {

  int BPM = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {

    Serial.print("Heart Rate : ");
    Serial.print(BPM);
    Serial.println(" BPM");
  }

  delay(20);
}