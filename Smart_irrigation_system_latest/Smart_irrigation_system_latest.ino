// #define SOIL_PIN A0       // Soil moisture sensor analog pin
// #define RED_LED 13         // Red LED for dry indication
// #define GREEN_LED 2       // Green LED for wet indication
// #define RELAY_PIN 8       // Relay control pin

// int soilValue = 0;        // Variable to store soil reading
// int dryThreshold = 500;   // You can adjust this based on testing

// void setup() {
//   Serial.begin(9600);
//   pinMode(RED_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);
//   pinMode(RELAY_PIN, OUTPUT);
//   digitalWrite(RELAY_PIN, LOW); // Ensure pump is off initially
// }

// void loop() {
//   soilValue = analogRead(SOIL_PIN);
//   Serial.print("Soil Moisture Value: ");
//   Serial.println(soilValue);

//   if (soilValue > dryThreshold) {
//     // Soil is dry
//     digitalWrite(RED_LED, HIGH);
//     digitalWrite(GREEN_LED, LOW);
//     digitalWrite(RELAY_PIN, HIGH); // Turn ON pump
//   } else {
//     // Soil is wet
//     digitalWrite(RED_LED, LOW);
//     digitalWrite(GREEN_LED, HIGH);
//     digitalWrite(RELAY_PIN, LOW); // Turn OFF pump
//   }

//   delay(1000); // Read every 1 second
// }

