#define TRIG 9
#define ECHO 10
#define BUZZER 8

long duration;
int distance;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance <= 20) {
    digitalWrite(BUZZER, HIGH); 
  } 
  else {
    digitalWrite(BUZZER, LOW);
  }

  delay(200);
}