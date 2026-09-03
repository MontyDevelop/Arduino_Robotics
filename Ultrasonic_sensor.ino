// Ultrasonic Sensor HC-SR04 📏

int trigPin = 9;
int echoPin = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {

  // 📢 Send a sound signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 👂 Listen for the returning sound
  long duration = pulseIn(echoPin, HIGH);

  // 📏 Calculate distance in centimeters
  int distance = duration * 0.034 / 2;

  // 🖥️ Show distance on Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}