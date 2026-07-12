#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int input1 = 3;
const int input2 = 2;
const int enablePin = 5;

const float hightemp = 25.0;   // Motor ON
const float lowtemp  = 23.0;   // Motor OFF

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
  analogWrite(enablePin, 0);
}

void loop() {
  float t = dht.readTemperature();
  
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  if (t >= hightemp) {
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
    analogWrite(enablePin, 150); 

  } else if (t <= lowtemp) {

    digitalWrite(input1, LOW);
    digitalWrite(input2, LOW);
    analogWrite(enablePin, 0);
  }
  delay(2000);
}
