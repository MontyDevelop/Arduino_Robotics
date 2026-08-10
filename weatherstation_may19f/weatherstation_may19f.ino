#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define RED 7
#define GREEN 6
#define BLUE 5

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Error reading sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.println(temp);

  if (temp < 25) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }

  else if (temp >= 25 && temp <= 30) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, LOW);
  }

  else {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  }

  delay(2000);
}