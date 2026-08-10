int sensorPin = 2;
int outputPin = 8;

bool state = false;  
bool lastSensorState = HIGH;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(outputPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  // Swipe detection (state change)
  if (sensorValue != lastSensorState) {
    delay(50); // debounce
    if (sensorValue == LOW) {  
      state = !state; // toggle
      digitalWrite(outputPin, state ? HIGH : LOW);
      Serial.println(state ? "ON" : "OFF");
    }
  }

  lastSensorState = sensorValue;
}
