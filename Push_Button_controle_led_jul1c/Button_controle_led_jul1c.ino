#define buttonPin 2
#define ledPin 8

bool ledState = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    delay(50);   // Debounce
  }

  lastButtonState = currentButtonState;
}