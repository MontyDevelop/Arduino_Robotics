int value;
int relay = 3;
int sensor = A0;
void setup(){
  pinMode(3, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop(){
  value = analogRead(sensor);
  if(value < 300)
  {
    digitalWrite(relay,HIGH);
  }
  else
  {
    digitalWrite(relay,LOW);
  }
Serial.println(value);  
delay(100);
}