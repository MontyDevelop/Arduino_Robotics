#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define OUT 8

int redValue;
int greenValue;
int blueValue;

void setup()
{
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // 20% Frequency Scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop()
{
  // Read RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redValue = pulseIn(OUT, LOW);

  // Read GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenValue = pulseIn(OUT, LOW);

  // Read BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueValue = pulseIn(OUT, LOW);

  Serial.print("R = ");
  Serial.print(redValue);

  Serial.print("   G = ");
  Serial.print(greenValue);

  Serial.print("   B = ");
  Serial.println(blueValue);

  // Color Detection
  if (redValue < greenValue && redValue < blueValue)
  {
    Serial.println("Detected Color : RED");
  }
  else if (greenValue < redValue && greenValue < blueValue)
  {
    Serial.println("Detected Color : GREEN");
  }
  else if (blueValue < redValue && blueValue < greenValue)
  {
    Serial.println("Detected Color : BLUE");
  }
  else
  {
    Serial.println("Detected Color : UNKNOWN");
  }

  Serial.println("-------------------------");

  delay(1000);
}