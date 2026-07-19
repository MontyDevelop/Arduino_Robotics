// Human-Like Fluid Robot Eyes Simulation (Inverted Colors: White Eyeball, Black Pupil)
// Drop-in code for your Adafruit_SH1106G setup

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

// --- Eye Dimensions & Mapping ---
const int EYE_W = 36;          // Width of each eye outer socket
const int EYE_H = 42;          // Height of each eye outer socket
const int LEFT_X = 16;         // Left eye X positioning
const int RIGHT_X = 76;        // Right eye X positioning
const int EYE_Y = 11;          // Vertical center line balance

// --- Physics & Kinematics Variables ---
float curX = 0.0;              // Current pupil X offset (smoothed)
float curY = 0.0;              // Current pupil Y offset (smoothed)
int tarX = 0;                  // Target gaze X point chosen by brain
int tarY = 0;                  // Target gaze Y point chosen by brain
float easeSpeed = 0.22;        // Human muscle tracking acceleration speed

// --- Micro-Expression State Engines ---
unsigned long lastGazeChange = 0;
unsigned long gazeDuration = 1500;
unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 4000;
bool blinking = false;
int blinkPhase = 0;            // 0=Closed, 1=Opening
unsigned long blinkActionTimer = 0;

void setup() {
  Wire.begin();
  
  // Initialize with your exact successful hardware address
  display.begin(0x3C, true);
  display.clearDisplay();
  display.display();
  
  // Pull thermal noise from an unconnected analog pin to randomize behavior sequences
  randomSeed(analogRead(A0)); 
}

void loop() {
  unsigned long now = millis();

  // -----------------------------------------------------------------
  // 1. SACCADIC GAZE CONTROLLER (Human Eye Shift Logic)
  // -----------------------------------------------------------------
  if (now - lastGazeChange > gazeDuration) {
    lastGazeChange = now;
    gazeDuration = random(600, 3200); // How long to hold focus on an object

    int choice = random(0, 100);
    if (choice < 35) {
      tarX = 0;
      tarY = 0;
    } else if (choice < 70) {
      // Dart gaze completely to the left or right side
      tarX = (random(0, 2) == 0) ? -8 : 8; // Slightly reduced to keep black pupil fully inside white eyeball
      tarY = random(-2, 3);
    } else if (choice < 90) {
      // Subtle organic glance shifting around
      tarX = random(-4, 5);
      tarY = random(-3, 4);
    } else {
      // Sharp look up towards top corners
      tarX = (random(0, 2) == 0) ? -5 : 5;
      tarY = -5;
    }
  }

  // Linear interpolation physics (Zeno's paradox easing curve)
  curX += (tarX - curX) * easeSpeed;
  curY += (tarY - curY) * easeSpeed;

  // -----------------------------------------------------------------
  // 2. BIOLOGICAL BLINK CONTROLLER
  // -----------------------------------------------------------------
  if (!blinking && (now - lastBlinkTime > blinkInterval)) {
    blinking = true;
    blinkPhase = 0; // Start down-stroke closing phase
    blinkActionTimer = now;
  }

  // -----------------------------------------------------------------
  // 3. GRAPHICS RENDERING PIPELINE
  // -----------------------------------------------------------------
  display.clearDisplay();

  if (blinking) {
    // Fast action parsing for realistic blink movement durations (~120ms total)
    if (blinkPhase == 0 && (now - blinkActionTimer > 60)) {
      blinkPhase = 1; // Start up-stroke opening phase
      blinkActionTimer = now;
    } else if (blinkPhase == 1 && (now - blinkActionTimer > 60)) {
      blinking = false; // Finished blink completely
      lastBlinkTime = now;
      blinkInterval = random(1500, 7000); // Random delay until next blink
    }

    // Render eyelids closing/opening based on phase state (White slits against black background)
    if (blinkPhase == 0) {
      // Slits closing down
      display.fillRoundRect(LEFT_X, EYE_Y + 18, EYE_W, 6, 2, SH110X_WHITE);
      display.fillRoundRect(RIGHT_X, EYE_Y + 18, EYE_W, 6, 2, SH110X_WHITE);
    } else {
      // Slits expanding open back to normal eye
      display.fillRoundRect(LEFT_X, EYE_Y + 12, EYE_W, 16, 4, SH110X_WHITE);
      display.fillRoundRect(RIGHT_X, EYE_Y + 12, EYE_W, 16, 4, SH110X_WHITE);
    }

  } else {
    // --- DRAW SOLID WHITE EYEBALLS ---
    int dynamicYOffset = (int)(curY * 0.3); // Biological micro-tilt
    
    display.fillRoundRect(LEFT_X, EYE_Y + dynamicYOffset, EYE_W, EYE_H, 16, SH110X_WHITE);
    display.fillRoundRect(RIGHT_X, EYE_Y + dynamicYOffset, EYE_W, EYE_H, 16, SH110X_WHITE);

    // --- DRAW BLACK PUPILS (Squash & Stretch Math) ---
    int pW = 14; // Base Pupil Width
    int pH = 22; // Base Pupil Height (Oval Shape)

    // Perspective Flattening: Slim down pupils if looking hard sideways
    if (abs(curX) > 5) {
      pW = 10; 
    }

    // Centering calculations with current physics offsets injection
    int leftIrisX = LEFT_X + (EYE_W / 2) + (int)curX;
    int rightIrisX = RIGHT_X + (EYE_W / 2) + (int)curX;
    int irisY = EYE_Y + (EYE_H / 2) + (int)curY;

    // Draw left and right oval pupils in BLACK to cut into the white eyeballs
    display.fillRoundRect(leftIrisX - (pW / 2), irisY - (pH / 2), pW, pH, 6, SH110X_BLACK);
    display.fillRoundRect(rightIrisX - (pW / 2), irisY - (pH / 2), pW, pH, 6, SH110X_BLACK);
    
    // --- White Catchlight/Glint inside the black pupil ---
    display.fillCircle(leftIrisX - 2, irisY - 4, 1, SH110X_WHITE);
    display.fillCircle(rightIrisX - 2, irisY - 4, 1, SH110X_WHITE);
  }

  display.display();
  delay(16); // Locks render cycle close to ~60 frames per second
}