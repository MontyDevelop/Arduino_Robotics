const int ldrPin = A0;
const int buzzer = 8;

int threshold = 20;

void setup()
{
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  int lightValue = analogRead(ldrPin);

  if(lightValue > threshold)
  {
    for(int f = 700; f <= 1500; f += 10)
    {
      tone(buzzer, f);
      delay(3);
    }

    for(int f = 1500; f >= 700; f -= 10)
    {
      tone(buzzer, f);
      delay(3);
    }
  }
  else
  {
    noTone(buzzer);
  }
}