#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 3   // Only one 8x8 matrix

#define DATA_PIN    11
#define CLK_PIN     12
#define CS_PIN      10

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

String newMessage = "HELLO!"; // Default message
char messageBuffer[100];      // Buffer for message

void setup() {
  Serial.begin(9600); // Open Serial Monitor at 9600 baud
  display.begin();
  display.setIntensity(2);  // Brightness 0-15
  display.displayClear();
  
  display.displayText(newMessage.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  Serial.println("Type a new message and press ENTER:");
}

void loop() {
  // Handle scrolling
  if (display.displayAnimate()) {
    display.displayReset();
  }

  // Check for new message from Serial Monitor
  if (Serial.available() > 0) {
    newMessage = Serial.readStringUntil('\n'); // Read until Enter key
    newMessage.trim(); // Remove spaces or newlines
    newMessage.toCharArray(messageBuffer, sizeof(messageBuffer));
    
    display.displayClear();
    display.displayText(messageBuffer, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}
