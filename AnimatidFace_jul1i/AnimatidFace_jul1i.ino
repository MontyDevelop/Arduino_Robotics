#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void drawFace(bool blinkLeft, bool blinkRight, bool smile)
{
  display.clearDisplay();

  // Face
  display.drawCircle(64, 32, 28, SH110X_WHITE);

  // Left Eye
  if(blinkLeft)
    display.drawLine(48, 24, 58, 24, SH110X_WHITE);
  else
    display.fillCircle(53, 24, 4, SH110X_WHITE);

  // Right Eye
  if(blinkRight)
    display.drawLine(70, 24, 80, 24, SH110X_WHITE);
  else
    display.fillCircle(75, 24, 4, SH110X_WHITE);

  // Mouth
  if(smile)
  {
    display.drawCircleHelper(64, 40, 10, 4, SH110X_WHITE);
    display.drawCircleHelper(64, 40, 10, 8, SH110X_WHITE);
  }
  else
  {
    display.drawLine(54, 44, 74, 44, SH110X_WHITE);
  }

  display.display();
}

void setup()
{
  display.begin(0x3C, true);
  display.clearDisplay();
}

void loop()
{
  // Normal Face
  drawFace(false, false, false);
  delay(1000);

  // Smile
  drawFace(false, false, true);
  delay(1000);

  // Blink
  drawFace(true, true, true);
  delay(250);

  drawFace(false, false, true);
  delay(1000);

  // Wink
  drawFace(true, false, true);
  delay(500);

  drawFace(false, false, true);
  delay(1000);

  // Straight Face
  drawFace(false, false, false);
  delay(1000);
}