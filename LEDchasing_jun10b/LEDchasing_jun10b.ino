const int leds[15] = {
  2,3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2
};

void setup()
{
  for(int i = 0; i < 15; i++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void allOff()
{
  for(int i = 0; i < 15; i++)
  {
    digitalWrite(leds[i], LOW);
  }
}

void loop()
{
  // Left -> Right
  for(int i = 0; i < 15; i++)
  {
    allOff();
    digitalWrite(leds[i], HIGH);
    delay(50);
  }

  // Right -> Left
  for(int i = 13; i > 0; i--)
  {
    allOff();
    digitalWrite(leds[i], HIGH);
    delay(50);
  }
}