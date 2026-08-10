#include <Servo.h>

Servo servoMotor;

int ldrLeft = A0;
int ldrRight = A1;

int pos = 90;

void setup() {

  servoMotor.attach(9);

  servoMotor.write(pos);

  Serial.begin(9600);
}

void loop() {

  int leftValue = analogRead(ldrLeft);

  int rightValue = analogRead(ldrRight);

  Serial.print("Left: ");
  Serial.print(leftValue);

  Serial.print(" Right: ");
  Serial.println(rightValue);

  // LEFT side light
  if (leftValue > rightValue + 50) {

    pos = pos - 1;

  }

  // RIGHT side light
  else if (rightValue > leftValue + 50) {

    pos = pos + 1;

  }

  // Limit angle
  pos = constrain(pos, 0, 180);

  servoMotor.write(pos);

  delay(15);
}