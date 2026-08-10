#include <Stepper.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define STEPS_PER_REV 2048

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

void setup() {
  myStepper.setSpeed(10);   // RPM
}

void loop() {
  myStepper.step(STEPS_PER_REV);   // 1 Full Rotation Clockwise
}