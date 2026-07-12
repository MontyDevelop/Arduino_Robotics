#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 11, 12);

struct SensorData {
  float temp;
  float hum;
  float ax;
  float ay;
  float az;
};

void setup() {
  Serial.begin(9600);
  driver.init();
}

void loop() {
  SensorData data;
  uint8_t buf[sizeof(SensorData)];
  uint8_t buflen = sizeof(buf);
  
  if (driver.recv(buf, &buflen)) {
    memcpy(&data, buf, sizeof(data));
    // Format for Serial Plotter
    Serial.print(data.temp);
    Serial.print(",");
    Serial.print(data.hum);
    Serial.print(",");
    Serial.print(data.ax);
    Serial.print(",");
    Serial.print(data.ay);
    Serial.print(",");
    Serial.println(data.az);
  }
}