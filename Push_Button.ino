// Push Button + LED 🔘💡

int buttonPin = 2;
int ledPin = 13;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);  // Button pressed → LED ON 💡
  }
  else {
    digitalWrite(ledPin, LOW);   // Button released → LED OFF 😴
  }
}