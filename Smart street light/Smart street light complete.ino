int LDRPin1= A0; 
int LDRPin2= A1; 
int LDRValue1=0; 
int LDRValue2=0; 
int led1 = 12;
int led2 = 11;
void setup()
{
Serial.begin(9600);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
}
void loop()
{
LDRValue1= analogRead(LDRPin1);
LDRValue2= analogRead(LDRPin2);
// Serial.print(“Analog reading = “);
// Serial.print(LDRValue);
if (LDRValue1 > 10)
{
digitalWrite(led1, HIGH);
digitalWrite(led2, LOW);
}
else if (LDRValue2 > 10)
{
digitalWrite(led2, HIGH);
digitalWrite(led1, LOW);
}
else{
  digitalWrite(led2, LOW);
digitalWrite(led1, LOW);
}
Serial.println(LDRValue1);
Serial.println(LDRValue2);
delay(1000);
}