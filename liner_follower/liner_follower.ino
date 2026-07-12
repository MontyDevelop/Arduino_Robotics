#define LEFT_IR   2
#define CENTER_IR 3
#define RIGHT_IR  4

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(CENTER_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {

  int L = digitalRead(LEFT_IR);
  int C = digitalRead(CENTER_IR);
  int R = digitalRead(RIGHT_IR);

  Serial.print(L);
  Serial.print(" ");
  Serial.print(C);
  Serial.print(" ");
  Serial.println(R);

  if (L == 1 && C == 0 && R == 1)
    forward();

  else if ((L == 0 && C == 1 && R == 1) || (L == 0 && C == 0 && R == 1))
    left();

  else if ((L == 1 && C == 1 && R == 0) || (L == 1 && C == 0 && R == 0))
    right();

  else if (L == 0 && C == 0 && R == 0)
    forward();

  else
    stopRobot();
}