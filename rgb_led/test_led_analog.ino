const int RED   = 9;
const int GREEN = 10;
const int BLUE  = 11;

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void setColor(int r, int g, int b)
{
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void loop()
{
  // Red -> Yellow
  for(int i = 0; i <= 255; i++)
  {
    setColor(255, i, 0);
    delay(15);
  }

  // Yellow -> Green
  for(int i = 255; i >= 0; i--)
  {
    setColor(i, 255, 0);
    delay(15);
  }

  // Green -> Cyan
  for(int i = 0; i <= 255; i++)
  {
    setColor(0, 255, i);
    delay(15);
  }

  // Cyan -> Blue
  for(int i = 255; i >= 0; i--)
  {
    setColor(0, i, 255);
    delay(15);
  }

  // Blue -> Purple
  for(int i = 0; i <= 255; i++)
  {
    setColor(i, 0, 255);
    delay(15);
  }

  // Purple -> Red
  for(int i = 255; i >= 0; i--)
  {
    setColor(255, 0, i);
    delay(15);
  }
}