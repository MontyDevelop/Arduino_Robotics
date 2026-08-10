int soundPin = 2;
int red = 9;
int green = 10;
int blue = 11;

void setup() {
  pinMode(soundPin, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.begin(9600);  // Serial monitor start
}

void loop() {
  int s = digitalRead(soundPin);

  Serial.print("Sound Value: ");
  Serial.println(s);   // Sensor value show karega

  if (s == LOW) { // Loud sound / clap
    Serial.println("Sound Detected!");

    analogWrite(red, random(0,255));
    analogWrite(green, random(0,255));
    analogWrite(blue, random(0,255));

    delay(120);
  } 
  else {
    Serial.println("No Sound");

    analogWrite(red, 0);
    analogWrite(green, 0);
    analogWrite(blue, 0);
  }

  delay(100);
}