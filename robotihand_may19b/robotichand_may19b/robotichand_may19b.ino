#include <Servo.h>

#define LEFT_BUTTON 3
#define RIGHT_BUTTON 2
#define SERVO_PIN 9

Servo myServo;

int angle = 90;   // Start from center

void setup()
{
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  myServo.attach(SERVO_PIN);
  myServo.write(angle);
}

void loop()
{
  // Left Button
  if (digitalRead(LEFT_BUTTON) == LOW)
  {
    if (angle > 0)
    {
      angle--;
      myServo.write(angle);
      delay(15);
    }
  }

  // Right Button
  if (digitalRead(RIGHT_BUTTON) == LOW)
  {
    if (angle < 180)
    {
      angle++;
      myServo.write(angle);
      delay(15);
    }
  }
}