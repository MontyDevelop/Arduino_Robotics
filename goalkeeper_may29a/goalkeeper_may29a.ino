#include <Servo.h>

Servo myServo;

const int leftButton = 2;
const int rightButton = 3;

void setup() {
  myServo.attach(9);

  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  myServo.write(90);   // start center
}

void loop() {

  // Left button → 0 degree
  if (digitalRead(leftButton) == LOW) {
    myServo.write(0);
  }

  // Right button → 180 degree
  if (digitalRead(rightButton) == LOW) {
    myServo.write(180);
  }
}