#define JOY_X A0

#define IN1 3
#define IN2 4

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int xValue = analogRead(JOY_X);

  Serial.println(xValue);

  if (xValue < 400) {      
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if (xValue > 600) {   
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else {                   
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  delay(50);
}