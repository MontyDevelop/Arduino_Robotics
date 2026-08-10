#define LDR A0
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6

int ldrValue;

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  ldrValue = analogRead(LDR);

  Serial.print("LDR Value : ");
  Serial.println(ldrValue);

  if (ldrValue > 500)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    Serial.println("Night - Street Light ON");
  }
  else
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    Serial.println("Day - Street Light OFF");
  }

  delay(500);
}