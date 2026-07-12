#define flamePin A1 // specify a name for port A1
#define ledPin 13 // set a name for port 13
int flame;
void setup() {
   Serial.begin(9600);
   pinMode(flamePin, INPUT);
   pinMode(ledPin, OUTPUT);
   analogWrite(flamePin, LOW);
}
void loop() {
   flame = analogRead(flamePin); // read sensor data
   Serial.print("Flame Sensor - "); // display data on the monitor
   Serial.println(flame);
   if (flame >= 100)
      digitalWrite(ledPin, HIGH);
   if (flame < 100)
      digitalWrite(ledPin, LOW);
}