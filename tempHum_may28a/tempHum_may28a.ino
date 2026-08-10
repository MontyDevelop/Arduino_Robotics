#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.clear();
}

void loop()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Sensor reading failed!");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");

    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  lcd.clear();

  // First Line
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C");

  // Second Line
  lcd.setCursor(0, 1);
  lcd.print("HUM:");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000);
}