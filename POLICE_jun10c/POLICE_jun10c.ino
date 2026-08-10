const int redLeds[]  = {2, 3, 4, 5, 6};
const int blueLeds[] = {7, 8, 9, 10, 11};

const int buzzer = 12;

unsigned long lastBlink = 0;
bool redPhase = true;
bool ledState = false;
int blinkCount = 0;

void setup()
{
  for(int i = 0; i < 5; i++)
  {
    pinMode(redLeds[i], OUTPUT);
    pinMode(blueLeds[i], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);
}

void loop()
{
  // ===== POLICE SIREN =====

  for(int freq = 700; freq <= 1500; freq += 10)
  {
    tone(buzzer, freq);

    updateLights();

    delay(4);
  }

  for(int freq = 1500; freq >= 700; freq -= 10)
  {
    tone(buzzer, freq);

    updateLights();

    delay(4);
  }
}

void updateLights()
{
  static unsigned long previousMillis = 0;

  if(millis() - previousMillis >= 70)
  {
    previousMillis = millis();

    ledState = !ledState;

    if(redPhase)
    {
      for(int i=0;i<5;i++)
      {
        digitalWrite(redLeds[i], ledState);
        digitalWrite(blueLeds[i], LOW);
      }
    }
    else
    {
      for(int i=0;i<5;i++)
      {
        digitalWrite(blueLeds[i], ledState);
        digitalWrite(redLeds[i], LOW);
      }
    }

    if(!ledState)
    {
      blinkCount++;

      if(blinkCount >= 4)
      {
        blinkCount = 0;
        redPhase = !redPhase;
      }
    }
  }
}