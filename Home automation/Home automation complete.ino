#include <Servo.h>
#define ledPin 7
#define servoPin 9
#define fan 10
Servo myservo;
int state = 0;
int pos1 = 0 ;
int pos2 = 180;

void setup() {
  
  myservo.attach(servoPin);
  pinMode(ledPin, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }
 if (state == '0') {
  digitalWrite(ledPin, LOW); 
  state = 0;
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  state = 0;
 } 
 else if (state == '2') {
  myservo.write(pos1);
  state = 0;                            
  } 
 else if (state == '3') {
  myservo.write(pos2);                                
  state = 0;
 }
}

