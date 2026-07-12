#define ldr_pin A0
#define led 13
// Create a variable ldr_value
int ldr_value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ldr_pin, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600); // This line is for communication in between arduino and computer
}

void loop() {
  // put your main code here, to run repeatedly:

  ldr_value = analogRead(ldr_pin); // We are reading the ldr_value as analog data coming from ldr_pin
  Serial.println(ldr_value);
  delay(500);

  if (ldr_value < 15){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
}
