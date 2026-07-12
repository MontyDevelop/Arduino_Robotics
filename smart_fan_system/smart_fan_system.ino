#include <Servo.h>

Servo myservo;

const int leftButton = 2;
const int rightButton = 3;
const int servoPin = 9;

int pos = 90;   // start center

void setup() {
  myservo.attach(servoPin);

  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  myservo.write(pos);
}

void loop() {

  // move left slowly
  if (digitalRead(leftButton) == LOW) {
    pos--;
    if (pos < 0) pos = 0;
    myservo.write(pos);
    delay(10);
  }

  // move right slowly
  if (digitalRead(rightButton) == LOW) {
    pos++;
    if (pos > 180) pos = 180;
    myservo.write(pos);
    delay(10);
  }
}