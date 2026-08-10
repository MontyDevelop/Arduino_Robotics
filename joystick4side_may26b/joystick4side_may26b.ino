int xPin = A0;
int yPin = A1;

int ledLeft = 2;
int ledUp = 3;
int ledRight = 4;
int ledDown = 5;

void setup() {
  pinMode(ledLeft, OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledDown, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  digitalWrite(ledLeft, LOW);
  digitalWrite(ledUp, LOW);
  digitalWrite(ledRight, LOW);
  digitalWrite(ledDown, LOW);

  if (xValue < 300) {
    digitalWrite(ledLeft, HIGH);
  }

  else if (xValue > 700) {
    digitalWrite(ledRight, HIGH);
  }

  else if (yValue > 700) {
    digitalWrite(ledUp, HIGH);
  }

  else if (yValue < 300) {
    digitalWrite(ledDown, HIGH);
  }

  delay(100);
}