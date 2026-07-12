// ============================================
// FLEX SENSOR CONTROLLED SERVO
// STEAMX Academy
// Calibrated for 100-300 range
// ============================================

#include <Servo.h>

#define FLEX_PIN        A0
#define SERVO_PIN       9

// ── Calibrated Values ───────────────────────
#define FLEX_STRAIGHT   100   // ← your actual value
#define FLEX_BENT       300   // ← your actual value

#define MIN_ANGLE       0
#define MAX_ANGLE       180
#define SMOOTH          0.3

Servo myServo;
float smoothAngle  = 90;
int   currentAngle = 90;

void setup() {
  myServo.attach(SERVO_PIN);
  myServo.write(90);
  Serial.begin(9600);
  Serial.println("Flex Sensor Ready!");
}

void loop() {
  int flexValue = analogRead(FLEX_PIN);

  // Map 100–300 → 0°–180°
  currentAngle = map(flexValue,
                     FLEX_STRAIGHT, FLEX_BENT,
                     MIN_ANGLE, MAX_ANGLE);

  // Safety constrain
  currentAngle = constrain(currentAngle, 0, 180);

  // Smooth movement
  smoothAngle = (smoothAngle * (1 - SMOOTH)) +
                (currentAngle * SMOOTH);

  myServo.write((int)smoothAngle);

  // Monitor output
  Serial.print("Flex: ");
  Serial.print(flexValue);
  Serial.print(" | Angle: ");
  Serial.println((int)smoothAngle);

  delay(20);
}