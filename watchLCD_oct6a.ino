#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set up the 16x2 LCD screen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- BUTTON PINS ---
const byte BUTTON_GREEN = 2; // START / STOP button
const byte BUTTON_YELLOW = 3; // MODE / RESET button

// --- STARTING TIME (10:30:00 AM) ---
const unsigned long START_HOUR   = 10;
const unsigned long START_MINUTE = 30;
const unsigned long START_SECOND = 0;
const unsigned long START_TOTAL_SECONDS = (START_HOUR * 3600UL) + (START_MINUTE * 60UL) + START_SECOND;

// --- WATCH STATE ---
byte currentMode = 0; // 0 = Clock Mode, 1 = Stopwatch Mode
bool updateScreenHeader = true;

// --- STOPWATCH TRACKER ---
bool isRacing = false;
unsigned long raceStartTime = 0;
unsigned long raceTimeRecorded = 0;

// --- TIMERS ---
unsigned long clockBirthTime = 0;
unsigned long lastDisplayTick = 0;

// --- BUTTON MEMORY & DEBOUNCE (Stops button "chatter") ---
const unsigned long BUTTON_WAIT = 40;       // 40ms to ignore button wobbles
const unsigned long HOLD_TO_RESET_TIME = 1000; // Hold yellow button 1 sec to reset

bool lastGreenState = HIGH;
unsigned long greenDebounceTimer = 0;

bool lastYellowState = HIGH;
unsigned long yellowDebounceTimer = 0;
unsigned long yellowPressStart = 0;
bool yellowWasHeld = false;

void setup() {
  lcd.init();
  lcd.backlight();

  // Internal pullups: when pressed, pin connects to GND (LOW)
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  clockBirthTime = millis();

  // --- FUN BOOT UP SCREEN ---
  lcd.setCursor(0, 0);
  lcd.print("  SUPER WATCH!  ");
  lcd.setCursor(0, 1);
  lcd.print(" READY TO RACE! ");
  delay(1200); // Friendly welcome pause
  lcd.clear();
}

void loop() {
  checkButtons();

  // Update the screen every 50 milliseconds (super smooth!)
  if (millis() - lastDisplayTick >= 50) {
    lastDisplayTick = millis();

    if (currentMode == 0) {
      drawClock();
    } else {
      drawStopwatch();
    }
  }
}

// ---------------------- HOW BUTTONS WORK ----------------------
void checkButtons() {
  unsigned long now = millis();

  // 1. GREEN BUTTON (Start / Stop the race)
  bool greenRead = digitalRead(BUTTON_GREEN);
  if (greenRead != lastGreenState && (now - greenDebounceTimer) > BUTTON_WAIT) {
    greenDebounceTimer = now;
    lastGreenState = greenRead;

    // Pressed down!
    if (greenRead == LOW && currentMode == 1) {
      if (!isRacing) {
        // GO! Start running
        raceStartTime = now - raceTimeRecorded;
        isRacing = true;
      } else {
        // PAUSE! Freeze time
        raceTimeRecorded = now - raceStartTime;
        isRacing = false;
      }
    }
  }

  // 2. YELLOW BUTTON (Tap = Change Mode | Hold = Reset Stopwatch)
  bool yellowRead = digitalRead(MODE_RESET_BUTTON_OR_YELLOW());
  if (yellowRead != lastYellowState && (now - yellowDebounceTimer) > BUTTON_WAIT) {
    yellowDebounceTimer = now;
    lastYellowState = yellowRead;

    if (yellowRead == LOW) {
      // Just pressed down
      yellowPressStart = now;
      yellowWasHeld = false;
    } else {
      // Released the button!
      if (!yellowWasHeld) {
        // Quick tap: Flip between Clock and Stopwatch
        currentMode = (currentMode == 0) ? 1 : 0;
        updateScreenHeader = true;
        lcd.clear();
      }
    }
  }

  // Check if child is holding the yellow button to RESET
  if (yellowRead == LOW && !yellowWasHeld) {
    if (now - yellowPressStart >= HOLD_TO_RESET_TIME) {
      yellowWasHeld = true;
      if (currentMode == 1) {
        // Reset the stopwatch to zero!
        isRacing = false;
        raceTimeRecorded = 0;
        lcd.setCursor(0, 1);
        lcd.print(" RESET TO ZERO! ");
        delay(350);
      }
    }
  }
}

// Helper to keep code clean
byte MODE_RESET_BUTTON_OR_YELLOW() {
  return BUTTON_YELLOW;
}

// ---------------------- SCREEN DRAWING ----------------------

void drawClock() {
  if (updateScreenHeader) {
    lcd.setCursor(0, 0);
    lcd.print("CLOCK  [INDIA]  ");
    updateScreenHeader = false;
  }

  // Calculate hours, minutes, and seconds
  unsigned long passedSeconds = (millis() - clockBirthTime) / 1000UL;
  unsigned long totalClockSeconds = (START_TOTAL_SECONDS + passedSeconds) % 86400UL;

  unsigned int hours = totalClockSeconds / 3600;
  unsigned int minutes = (totalClockSeconds % 3600) / 60;
  unsigned int seconds = totalClockSeconds % 60;

  // Make a neat text box: "TIME: 10:30:15   "
  char displayLine[17];
  snprintf(displayLine, sizeof(displayLine), "TIME:  %02u:%02u:%02u ", hours, minutes, seconds);

  lcd.setCursor(0, 1);
  lcd.print(displayLine);
}

void drawStopwatch() {
  if (updateScreenHeader) {
    lcd.setCursor(0, 0);
    lcd.print("RACE TRACK!     ");
    updateScreenHeader = false;
  }

  unsigned long totalRaceTime = isRacing ? (millis() - raceStartTime) : raceTimeRecorded;

  unsigned int minutes = (totalRaceTime / 60000UL) % 60;
  unsigned int seconds = (totalRaceTime / 1000UL) % 60;
  unsigned int centis  = (totalRaceTime % 1000UL) / 10; // Hundredths of a second (00-99)

  // Status icon: [>] running, [||] paused
  const char* statusTag = isRacing ? ">>" : "||";

  char displayLine[17];
  snprintf(displayLine, sizeof(displayLine), "%s  %02u:%02u.%02u   ", statusTag, minutes, seconds, centis);

  lcd.setCursor(0, 1);
  lcd.print(displayLine);
}
