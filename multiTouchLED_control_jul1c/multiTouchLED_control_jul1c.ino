#define TOUCH1 2
#define TOUCH2 3
#define TOUCH3 4
#define TOUCH4 5

#define LED1 8
#define LED2 9
#define LED3 10
#define LED4 11

bool ledState1 = LOW;
bool ledState2 = LOW;
bool ledState3 = LOW;
bool ledState4 = LOW;

bool lastTouch1 = LOW;
bool lastTouch2 = LOW;
bool lastTouch3 = LOW;
bool lastTouch4 = LOW;

void setup() {

  pinMode(TOUCH1, INPUT);
  pinMode(TOUCH2, INPUT);
  pinMode(TOUCH3, INPUT);
  pinMode(TOUCH4, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void loop() {

  bool touch1 = digitalRead(TOUCH1);
  bool touch2 = digitalRead(TOUCH2);
  bool touch3 = digitalRead(TOUCH3);
  bool touch4 = digitalRead(TOUCH4);

  // Touch 1
  if (touch1 == HIGH && lastTouch1 == LOW) {
    ledState1 = !ledState1;
    digitalWrite(LED1, ledState1);
    delay(100);
  }

  // Touch 2
  if (touch2 == HIGH && lastTouch2 == LOW) {
    ledState2 = !ledState2;
    digitalWrite(LED2, ledState2);
    delay(100);
  }

  // Touch 3
  if (touch3 == HIGH && lastTouch3 == LOW) {
    ledState3 = !ledState3;
    digitalWrite(LED3, ledState3);
    delay(100);
  }

  // Touch 4
  if (touch4 == HIGH && lastTouch4 == LOW) {
    ledState4 = !ledState4;
    digitalWrite(LED4, ledState4);
    delay(100);
  }

  lastTouch1 = touch1;
  lastTouch2 = touch2;
  lastTouch3 = touch3;
  lastTouch4 = touch4;
}