#define CLK 3
#define DT  2
#define LED 9

int brightness = 0;   // 0-255
int lastCLK;

void setup()
{
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(LED, OUTPUT);

  lastCLK = digitalRead(CLK);

  analogWrite(LED, brightness);

  Serial.begin(9600);
}

void loop()
{
  int currentCLK = digitalRead(CLK);

  if(currentCLK != lastCLK && currentCLK == HIGH)
  {
    // Clockwise
    if(digitalRead(DT) != currentCLK)
    {
      brightness += 25;   // ~10% increase
    }
    // Anti-clockwise
    else
    {
      brightness -= 25;   // ~10% decrease
    }

    // Limit brightness
    brightness = constrain(brightness, 0, 255);

    analogWrite(LED, brightness);

    Serial.print("Brightness: ");
    Serial.print(map(brightness, 0, 255, 0, 100));
    Serial.println("%");
  }

  lastCLK = currentCLK;
}