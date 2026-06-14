const int RED = 9;
const int GREEN = 10;
const int BLUE = 11;

void setColor(int r, int g, int b)
{
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop()
{
  setColor(255,0,0);     // Red
  delay(100);

  setColor(255,255,0);   // Yellow
  delay(100);

  setColor(0,255,0);     // Green
  delay(100);

  setColor(0,255,255);   // Cyan
  delay(100);

  setColor(0,0,255);     // Blue
  delay(100);

  setColor(255,0,255);   // Purple
  delay(100);
}