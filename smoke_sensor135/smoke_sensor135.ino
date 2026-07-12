const int gasPin = A0;   // MQ-135 analog output connected to A0
int gasValue = 0;

void setup() {
  Serial.begin(9600);    // Start Serial Monitor
}

void loop() {
  gasValue = analogRead(gasPin);  // Read analog value
  Serial.print("Air Quality Value: ");
  Serial.println(gasValue);       // Print value to Serial Monitor

  if (gasValue > 400) {
  Serial.println("Clean Air");
} else if (gasValue > 250) {
  Serial.println("Moderate Pollution");
} else {
  Serial.println("High Pollution or Smoke Detected!");
}


  delay(1000);  // 1 second delay
}
