const int RED = 9;
const int GREEN = 10;
const int BLUE = 11;

const int soundPin = A0;

int colorIndex = 0;
int threshold = 550;   // adjust if needed

void setColor(int r, int g, int b)
{c:\Users\VINITA\OneDrive\Documents\Arduino\espwirelessT_jun6b
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void nextColor()
{
  switch(colorIndex)
  {
    case 0: setColor(255,0,0); break;       // Red
    case 1: setColor(0,255,0); break;       // Green
    case 2: setColor(0,0,255); break;       // Blue
    case 3: setColor(255,0,255); break;     // Purple
    case 4: setColor(255,255,0); break;     // Yellow
    case 5: setColor(0,255,255); break;     // Cyan
    case 6: setColor(255,255,255); break;   // White
  }

  colorIndex++;
  if(colorIndex > 6)
    colorIndex = 0;
}

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  int sound = analogRead(soundPin);

  Serial.println(sound);

  if(sound > threshold)
  {
    nextColor();

    // debounce
    delay(150);
  }
}