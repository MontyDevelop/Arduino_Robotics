int value;
int relay = 3;
int sensor = 6;
void setup(){
  // pinMode(3, OUTPUT);
  // pinMode(6, INPUT);
}

void loop(){
  value = digitalRead(sensor);
  if(value == HIGH)
  {
    digitalWrite(relay,LOW);
  }
  else
  {
    digitalWrite(relay,HIGH);
  }
delay(400);
}