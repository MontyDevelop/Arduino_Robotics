#define BUZZER 9

// Musical Notes
#define C 262
#define D 294
#define E 330
#define F 349
#define G 392
#define A 440
#define AS 466
#define B 494
#define CH 523   // High C

// Happy Birthday Song
int notes[] = {
  C, C, D, C, F, E,
  C, C, D, C, G, F,
  C, C, CH, A, F, E, D,
  AS, AS, A, F, G, F
};

// Time for each note
int beats[] = {
  300, 300, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 600, 1200,
  300, 300, 600, 600, 600, 1200
};

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {

  // Total number of notes
  int size = 25;

  // Play notes one by one
  for (int i = 0; i < size; i++) {

    tone(BUZZER, notes[i]); // Play sound
    delay(beats[i]);        // Wait

    noTone(BUZZER);         // Stop sound
    delay(50);              // Small gap
  }

  delay(3000); // Wait before replay
}