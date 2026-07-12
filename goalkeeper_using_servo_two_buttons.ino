#include <Servo.h>
Servo myServo;
#define leftButton 2
#define rightButton 3
#define servoPin 9
int angle = 90;   

void setup() {
  myServo.attach(servoPin);
  myServo.write(angle);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(rightButton) == LOW) {
    angle = angle - 5;
    if (angle < 0) {
      angle = 0;
    }
    myServo.write(angle);
    delay(50);
  }
  if (digitalRead(leftButton) == LOW) {
    angle = angle + 5;
    if (angle > 180) {
      angle = 180;
    }
    myServo.write(angle);
    delay(50);
  }
}