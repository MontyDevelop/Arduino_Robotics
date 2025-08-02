// HC-SR04 Ultrasonic Distance Measurement Example
// Wiring: Trig -> pin 9, Echo -> pin 10

const int trigPin = 9;   // Trigger pin
const int echoPin = 10;  // Echo pin

void setup() {
  Serial.begin(9600);            // Start serial communication for output
  pinMode(trigPin, OUTPUT);      // Set trigger pin as output
  pinMode(echoPin, INPUT);       // Set echo pin as input
}

void loop() {
  long duration;
  float distanceCm;

  // 1. Send a 10-microsecond pulse on Trig to start measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);              // Small delay to settle
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);             // 10µs pulse
  digitalWrite(trigPin, LOW);

  // 2. Read the time for echo to return
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms to avoid hanging

  // If no echo (duration == 0) we can treat as out of range
  if (duration == 0) {
    Serial.println("Out of range or no object detected");
  } else {
    // 3. Calculate distance: Sound speed ~343 m/s => 29.1 µs per cm round-trip
    distanceCm = (duration / 2.0) / 29.1; // divide by 2 for one-way

    // 4. Print result
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }

  delay(200); // Small delay before next reading
}
