#define X_PIN A0
#define Y_PIN A1

int xValue, yValue;

void setup() {
  Serial.begin(9600);
}

void loop() {

  xValue = analogRead(X_PIN);
  yValue = analogRead(Y_PIN);

  if (xValue < 300) {
    Serial.println("LEFT");
  }
  else if (xValue > 700) {
    Serial.println("RIGHT");
  }
  else if (yValue < 300) {
    Serial.println("UP");
  }
  else if (yValue > 700) {
    Serial.println("DOWN");
  }
  else {
    Serial.println("CENTER");
  }

  delay(200);
}