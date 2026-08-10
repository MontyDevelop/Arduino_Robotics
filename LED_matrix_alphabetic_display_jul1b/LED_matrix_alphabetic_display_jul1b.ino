#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1

#define DATA_PIN 11
#define CS_PIN   10
#define CLK_PIN  13

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char letter = 'A';

void setup() {
  display.begin();
  display.setIntensity(5);
  display.displayClear();
}

void loop() {

  char text[2];
  text[0] = letter;
  text[1] = '\0';

  display.displayClear();
  display.displayText(text, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

  while (!display.displayAnimate());

  letter++;

  if (letter > 'Z') {
    letter = 'A';
  }
}