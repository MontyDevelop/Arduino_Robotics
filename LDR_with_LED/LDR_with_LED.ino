int data = A0;
int collected_data = 0;
int led = 12;

void setup() {
  pinMode(data, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600); // This is the speed of data travel through ldr to computer.
}

void loop() {
  collected_data = analogRead(data);
  Serial.println(collected_data);
  if (collected_data > 100){
    digitalWrite(led, LOW);
  }else{
    digitalWrite(led,HIGH);
  }
}
