#include <Adafruit_NeoPixel.h>

#define LED_PIN     6
#define LED_COUNT   12
#define TOUCH_PIN   2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
long hue = 0;

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  strip.begin();
  strip.show();
}

void loop() {
  if (digitalRead(TOUCH_PIN) == HIGH) {
    rainbowBurst();
  } else {
    strip.clear();
    strip.show();
  }
}

void rainbowBurst() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixelHue = hue + (i * 65536L / strip.numPixels());
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show();
  hue += 800;
  delay(10);
}