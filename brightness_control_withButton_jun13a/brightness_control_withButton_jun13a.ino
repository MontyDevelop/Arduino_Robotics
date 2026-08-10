#define LED_PIN 9
#define LEFT_BUTTON A0
#define RIGHT_BUTTON A1

int level = 0;   // 0 to 10

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  analogWrite(LED_PIN, 0);
}

void loop()
{
  // Increase Brightness
  if(digitalRead(RIGHT_BUTTON) == LOW)
  {
    if(level < 10)
      level++;

    analogWrite(LED_PIN, level * 25.5);

    delay(200);
  }

  // Decrease Brightness
  if(digitalRead(LEFT_BUTTON) == LOW)
  {
    if(level > 0)
      level--;

    analogWrite(LED_PIN, level * 25.5);

    delay(200);
  }
}