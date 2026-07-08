#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1

#define DATA_PIN 11
#define CS_PIN   10
#define CLK_PIN  13

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char letter = 'A';

void setup()
{
  matrix.begin();

  matrix.setIntensity(5);

  matrix.displayClear();
}

void loop()
{
  char text[2];

  text[0] = letter;

  text[1] = '\0';

  matrix.displayClear();

  matrix.displayText(text, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

  while (!matrix.displayAnimate());

  delay(1000);

  letter++;

  if(letter > 'Z')
  {
    letter = 'A';
  }
}