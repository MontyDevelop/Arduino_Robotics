#define RED   9
#define GREEN 10
#define BLUE  11

#define BUTTON A0

int colorIndex = 0;
bool lastButtonState = HIGH;

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

  pinMode(BUTTON, INPUT_PULLUP);

  setColor(0,0,0);
}

void loop()
{
  bool buttonState = digitalRead(BUTTON);

  // Button Press Detection
  if(buttonState == LOW && lastButtonState == HIGH)
  {
    colorIndex++;

    if(colorIndex > 7)
      colorIndex = 0;

    switch(colorIndex)
    {
      case 0: setColor(255,0,0); break;       // Red
      case 1: setColor(0,255,0); break;       // Green
      case 2: setColor(0,0,255); break;       // Blue
      case 3: setColor(255,255,0); break;     // Yellow
      case 4: setColor(0,255,255); break;     // Cyan
      case 5: setColor(255,0,255); break;     // Purple
      case 6: setColor(255,255,255); break;   // White
      case 7: setColor(0,0,0); break;         // OFF
    }

    delay(200); // debounce
  }

  lastButtonState = buttonState;
}