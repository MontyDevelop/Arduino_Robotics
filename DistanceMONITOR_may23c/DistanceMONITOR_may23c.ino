

#define trigPin 9
#define echoPin 10
#define LED 6
#define Buzzer 8

long duration;
float distance;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Distance Meter");

  delay(2000);
  lcd.clear();
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);


  distance = duration * 0.034 / 2;

  
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Distance:");

  lcd.setCursor(0,1);
  lcd.print(distance);
  lcd.print(" cm");

  delay(500);
}