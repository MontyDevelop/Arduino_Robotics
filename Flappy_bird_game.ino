#define TRIG_PIN 9
#define ECHO_PIN 10

long duration;
float distance;

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration * 0.0343 / 2;

  if (distance > 50)
  {
    distance = 50;
  }

  if (distance < 5)
  {
    distance = 5;
  }

  Serial.println(distance);

  delay(40);
}