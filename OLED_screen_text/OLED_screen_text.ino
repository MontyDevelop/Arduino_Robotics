#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);

#define NUM_CONFETTI 20

int x[NUM_CONFETTI];
int y[NUM_CONFETTI];
int speedY[NUM_CONFETTI];

void setup() {
  display.begin(0x3C, true);

  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_CONFETTI; i++) {
    x[i] = random(0, 128);
    y[i] = random(-64, 0);
    speedY[i] = random(1, 4);
  }
}

void loop() {
  display.clearDisplay();

  // Text
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 22);
  display.println("RBC WINS");

  // Confetti
  for (int i = 0; i < NUM_CONFETTI; i++) {
    display.fillCircle(x[i], y[i], 1, SH110X_WHITE);

    y[i] += speedY[i];

    if (y[i] > 64) {
      y[i] = random(-20, 0);
      x[i] = random(0, 128);
    }
  }

  display.display();
  delay(60);
}