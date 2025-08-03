// Define pin numbers
#define TRIG_PIN 5
#define ECHO_PIN 6

// Variable to store duration and distance
long duration;
int distance;

void setup() {
  // Start the serial monitor
  Serial.begin(9600);

  // Set the trigPin as output and echoPin as input
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Clear the trigPin (LOW for 2 microseconds)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger the sensor (HIGH for 10 microseconds)
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance (speed of sound is ~343 m/s)
  distance = duration * 0.034 / 2; // cm

  // Print distance in cm
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // wait half a second before next reading
}
