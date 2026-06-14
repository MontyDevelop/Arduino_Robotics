const int leds[15] = {
  2,3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2
};

const int btnRight = A3;   // LED ko right le jane ke liye
const int btnLeft  = A4;   // LED ko left le jane ke liye

int pos = 0; // LED1 se start

bool lastRightState = HIGH;
bool lastLeftState  = HIGH;

void showLED()
{
  for(int i=0;i<15;i++)
  {
    digitalWrite(leds[i], LOW);
  }

  digitalWrite(leds[pos], HIGH);
}

void setup()
{
  for(int i=0;i<15;i++)
  {
    pinMode(leds[i], OUTPUT);
  }

  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);

  showLED();
}

void loop()
{
  bool rightState = digitalRead(btnRight);
  bool leftState  = digitalRead(btnLeft);

  // Right move
  if(rightState == LOW && lastRightState == HIGH)
  {
    if(pos < 14)
      pos++;

    showLED();
    delay(50);
  }

  // Left move
  if(leftState == LOW && lastLeftState == HIGH)
  {
    if(pos > 0)
      pos--;

    showLED();
    delay(50);
  }

  lastRightState = rightState;
  lastLeftState = leftState;
}