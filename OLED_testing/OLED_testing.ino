// OLED Display Only Isolation Test
// No sensors required - Tests screen initialization and rendering

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Your exact working display initialization
Adafruit_SH1106G display(128, 64, &Wire, -1);

int boxX = 0; // Variable to move a box across the screen

void setup() {
  Serial.begin(9600);
  Serial.println("Starting OLED Isolation Test...");

  // Force initialize the I2C bus wire lines
  Wire.begin(); 

  // Initialize the SH1106 display over I2C at address 0x3C
  if (!display.begin(0x3C, true)) {
    Serial.println("OLED allocation failed! Check your wires.");
    for(;;); // Halt here if the screen hardware isn't found
  }

  Serial.println("OLED found and running successfully!");
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
}

void loop() {
  display.clearDisplay();

  // 1. Draw a boundary frame border around the screen edge
  display.drawRect(0, 0, 128, 64, SH110X_WHITE);

  // 2. Display Static Text Elements
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.print("OLED TEST");

  display.setTextSize(1);
  display.setCursor(15, 34);
  display.print("Hardware Online");

  // 3. Draw a moving indicator box at the bottom to test active refresh rate
  display.fillRect(boxX, 48, 12, 8, SH110X_WHITE);

  // Move the box across rows, wrap back around if it leaves the right screen edge
  boxX += 2;
  if (boxX > 110) {
    boxX = 15;
  }

  // Render buffer data to screen hardware pixels
  display.display();
  delay(20); 
}