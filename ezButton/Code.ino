// Resource: https://arduinogetstarted.com/tutorials/arduino-button-library

#include <ezButton.h>

ezButton button(A0);
int button_data = 0;
// int ledG = 2;
// int ledR = 4;
// int ledB = 3;

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50);
  button.setCountMode(COUNT_FALLING);
  // pinMode(ledG, OUTPUT);
  // pinMode(ledR, OUTPUT);
  // pinMode(ledB, OUTPUT);
}

void loop() {
  button.loop();
  long clicks = button.getCount();
  Serial.println(clicks);
  // if (clicks == 1) {
  //   digitalWrite(ledG, HIGH);
  //   digitalWrite(ledR, LOW);
  //   digitalWrite(ledB, LOW);
  // } else if (clicks == 2) {
  //   digitalWrite(ledG, LOW);
  //   digitalWrite(ledB, HIGH);
  //   digitalWrite(ledR, LOW);
  // } else if (clicks == 3) {
  //   digitalWrite(ledG, LOW);
  //   digitalWrite(ledB, LOW);
  //   digitalWrite(ledR, HIGH);
  // } else if (clicks == 4) {
  //   digitalWrite(ledG, HIGH);
  //   digitalWrite(ledR, HIGH);
  //   digitalWrite(ledB, HIGH);
  // } else if (clicks == 5) {
  //   digitalWrite(ledG, LOW);
  //   digitalWrite(ledR, LOW);
  //   digitalWrite(ledB, LOW);
  // }
}
