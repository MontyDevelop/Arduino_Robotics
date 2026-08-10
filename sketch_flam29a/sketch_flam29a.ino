int soundPin = A0;

int red = 9;
int green = 10;
int blue = 11;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int soundValue = analogRead(soundPin);
  Serial.println(soundValue);

  // Low sound → BLUE
  if(soundValue < 300) {
    setColor(0, 0, 255);
  }

  // Medium sound → GREEN
  else if(soundValue < 600) {
    setColor(0, 255, 0);
  }

  // High sound → RED
  else if(soundValue < 900) {
    setColor(255, 0, 0);
  }

  // Very high → FLASH WHITE
  else {
    setColor(255, 255, 255);
  }

  delay(20);
}

void setColor(int r, int g, int b) {
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}
