#include <DHT.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <RH_ASK.h>
#include <SPI.h>

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
Adafruit_MPU6050 mpu;
RH_ASK driver(2000, 11, 12);

struct SensorData {
  float temp;
  float hum;
  float ax;
  float ay;
  float az;
} data;

void setup() {
  Serial.begin(9600);
  dht.begin();
  mpu.begin();
  driver.init();
}

void loop() {
  data.temp = dht.readTemperature();
  data.hum = dht.readHumidity();
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  data.ax = a.acceleration.x;
  data.ay = a.acceleration.y;
  data.az = a.acceleration.z;
  
  driver.send((uint8_t *)&data, sizeof(data));
  driver.waitPacketSent();
  delay(100);
}