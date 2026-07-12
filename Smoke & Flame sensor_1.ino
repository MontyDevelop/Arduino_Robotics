int smoke = 0;
int flame = 0;
int smoke_pin = A0;
int flame_pin = 4; 
int buzzer = 12;
void setup()
{
Serial.begin(9600);
pinMode(smoke_pin,INPUT);
pinMode(flame_pin,INPUT);
pinMode(buzzer,OUTPUT);
}
void loop()
{
// Getting the data from smoke and flame sensor
smoke = analogRead(smoke_pin);
flame = digitalRead(flame_pin);
Serial.println("Smoke level----");
Serial.println(smoke);
Serial.println("Flame status----");
Serial.println(flame);
// Now condition need to satisfy to on the buzzer
if(smoke > 300 or flame == 0){
digitalWrite(buzzer,HIGH);
}
else
{
digitalWrite(buzzer,LOW);
}
delay(500);
}

