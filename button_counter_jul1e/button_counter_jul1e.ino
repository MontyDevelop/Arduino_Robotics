#include <ezButton.h>

#define BUTTON_PIN 2

ezButton button(BUTTON_PIN);

int count = 0;

void setup() {

  Serial.begin(9600);

  button.setDebounceTime(50);

  Serial.println("Button Counter Started");
}

void loop() {

  button.loop();

  if (button.isPressed()) {

    count++;

    Serial.print("Button Count = ");
    Serial.println(count);
  }
}