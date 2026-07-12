#define Potentiometer A0
#define led 3
int values1;
int values2;

void setup() {
  // put your setup code here, to run once:
  pinMode(Potentiometer, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  values1 = analogRead(Potentiometer);
  values2 = map(values1, 0,1023, 0, 255);
  Serial.println(values2);
  analogWrite(led, values2);
  delay(100);
}

//Resource: https://www.arduino.cc/reference/en/language/functions/math/map/
