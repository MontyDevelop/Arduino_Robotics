#define Potentiometer A0
#define led 2
int values;

void setup() {
  // put your setup code here, to run once:
  pinMode(Potentiometer, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  values = analogRead(Potentiometer);
  Serial.println(values);
  analogWrite(led, values);
  delay(100);
}
