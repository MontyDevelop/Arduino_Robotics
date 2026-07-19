#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the type of hardware you have. 
// Most common pre-made modules use FC16_HW. If your text appears upside down or reversed,
// change this to GENERIC_HW, ICSTATION_HW, or PAROLA_HW.
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Set how many 8x8 modules you have chained together
#define MAX_DEVICES 3 

// Define the Chip Select pin
#define CS_PIN 10

// Create an instance of the Parola display
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// The message you want to scroll
const char* newMessage = "Hello World! Custom Scrolling Text with 3 Matrices.";

void setup() {
  // Initialize the display object
  myDisplay.begin();
  
  // Set the brightness (0 is dimmest, 15 is brightest)
  myDisplay.setIntensity(4);
  
  // Clear the display
  myDisplay.displayClear();

  // Configure the text scrolling effect
  // Arguments: Text array, Alignment, Speed (lower = faster), Pause time, Effect In, Effect Out
  myDisplay.displayText(newMessage, PA_LEFT, 60, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  // Animates the display and handles the text scrolling loop automatically
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}