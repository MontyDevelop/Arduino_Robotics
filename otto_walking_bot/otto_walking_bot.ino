#include <Otto.h>

Otto Otto;

// Servo Pins
#define PIN_LL 2
#define PIN_RL 3
#define PIN_LF 4
#define PIN_RF 5

// Ultrasonic Pins
#define TRIG_PIN 8
#define ECHO_PIN 9

#define BUZZER_PIN 13

long duration;
int distance;

void setup() {

  Otto.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, BUZZER_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Otto.home();
  delay(1000);
}

void loop() {

  distance = getDistance();

  if (distance > 20 || distance == 0) {

    // Path is clear
    Otto.walk(1, 700, 1);

  }
  else {

    // Obstacle detected
    Otto.sing(S_surprise);

    Otto.home();
    delay(300);

    // Move backward
    Otto.walk(2, 700, -1);

    delay(300);

    // Turn randomly
    if (random(2) == 0) {
      Otto.turn(2, 700, 1);
    } else {
      Otto.turn(2, 700, -1);
    }

    delay(300);
  }
}

int getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return 0;

  return duration * 0.034 / 2;
}