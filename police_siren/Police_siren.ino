// Buzzers
#define BUZZER1 2
#define BUZZER2 3

// Red LEDs
#define RED1 4
#define RED2 5
#define RED3 6

// Blue LEDs
#define BLUE1 7
#define BLUE2 8
#define BLUE3 9

// White LEDs
#define WHITE1 10
#define WHITE2 11
#define WHITE3 12

void setup() {

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  for(int pin = 4; pin <= 12; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void allOff() {
  for(int pin = 4; pin <= 12; pin++) {
    digitalWrite(pin, LOW);
  }
}

void loop() {

  // Siren sweep up
  for(int freq = 700; freq <= 1400; freq += 20) {

    tone(BUZZER1, freq);
    tone(BUZZER2, freq);

    // Double Red Flash
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
    digitalWrite(RED3, HIGH);

    digitalWrite(WHITE1, HIGH);
    digitalWrite(WHITE2, HIGH);
    digitalWrite(WHITE3, HIGH);

    delay(25);

    allOff();

    delay(25);

    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
    digitalWrite(RED3, HIGH);

    delay(25);

    allOff();

    delay(25);
  }

  // Siren sweep down
  for(int freq = 1400; freq >= 700; freq -= 20) {

    tone(BUZZER1, freq);
    tone(BUZZER2, freq);

    // Double Blue Flash
    digitalWrite(BLUE1, HIGH);
    digitalWrite(BLUE2, HIGH);
    digitalWrite(BLUE3, HIGH);

    digitalWrite(WHITE1, HIGH);
    digitalWrite(WHITE2, HIGH);
    digitalWrite(WHITE3, HIGH);

    delay(25);

    allOff();

    delay(25);

    digitalWrite(BLUE1, HIGH);
    digitalWrite(BLUE2, HIGH);
    digitalWrite(BLUE3, HIGH);

    delay(25);

    allOff();

    delay(25);
  }
}