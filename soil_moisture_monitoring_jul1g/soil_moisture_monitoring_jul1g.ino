#define SOIL_SENSOR A0

int moistureValue;
int moisturePercentage;

void setup()
{
  Serial.begin(9600);
  Serial.println("Soil Moisture Monitoring System");
}

void loop()
{
  moistureValue = analogRead(SOIL_SENSOR);


  moisturePercentage = map(moistureValue, 1023, 300, 0, 100);


  moisturePercentage = constrain(moisturePercentage, 0, 100);

 
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");


  if (moisturePercentage < 30)
  {
    Serial.println("Soil is Dry");
  }
  else if (moisturePercentage < 70)
  {
    Serial.println("Soil is Moist");
  }
  else
  {
    Serial.println("Soil is Wet");
  }

  Serial.println("------------------------");

  delay(1000);
}