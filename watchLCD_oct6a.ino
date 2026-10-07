#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define START_STOP_BUTTON 2
#define MODE_RESET_BUTTON 3

#define START_HOUR 10
#define START_MINUTE 30
#define START_SECOND 0


byte mode = 0;


unsigned long watchStartMillis;


bool stopwatchRunning = false;

unsigned long stopwatchStartMicros = 0;
unsigned long stopwatchElapsedMicros = 0;

bool lastStartButton = HIGH;
bool lastModeButton = HIGH;

unsigned long firstPressTime = 0;

byte modeButtonPressCount = 0;

#define DOUBLE_PRESS_TIME 500


unsigned long lastLCDUpdate = 0;


void setup()
{
  lcd.init();
  lcd.backlight();

  pinMode(START_STOP_BUTTON, INPUT_PULLUP);
  pinMode(MODE_RESET_BUTTON, INPUT_PULLUP);

  watchStartMillis = millis();

  lcd.setCursor(0, 0);
  lcd.print("SMART WATCH");

  lcd.setCursor(0, 1);
  lcd.print("INDIA TIME");

  delay(1000);

  lcd.clear();
}


void loop()
{
  checkStartStopButton();
  checkModeResetButton();

  // Check double press
  checkDoublePress();

  // Update display
  if (millis() - lastLCDUpdate >= 50)
  {
    lastLCDUpdate = millis();

    if (mode == 0)
    {
      showRealTime();
    }
    else
    {
      showStopwatch();
    }
  }
}


void checkStartStopButton()
{
  bool buttonState = digitalRead(START_STOP_BUTTON);

  if (lastStartButton == HIGH && buttonState == LOW)
  {
    // Only work in stopwatch mode
    if (mode == 1)
    {
      if (stopwatchRunning == false)
      {
        // START
        stopwatchStartMicros =
          micros() - stopwatchElapsedMicros;

        stopwatchRunning = true;
      }
      else
      {
        // STOP
        stopwatchElapsedMicros =
          micros() - stopwatchStartMicros;

        stopwatchRunning = false;
      }
    }
  }

  lastStartButton = buttonState;
}


void checkModeResetButton()
{
  bool buttonState = digitalRead(MODE_RESET_BUTTON);

  if (lastModeButton == HIGH && buttonState == LOW)
  {
    unsigned long currentTime = millis();

    // First press
    if (modeButtonPressCount == 0)
    {
      firstPressTime = currentTime;
      modeButtonPressCount = 1;
    }

    // Second press
    else
    {
      if (currentTime - firstPressTime <= DOUBLE_PRESS_TIME)
      {
        // DOUBLE PRESS
        changeMode();

        modeButtonPressCount = 0;
      }
      else
      {
        // Previous press was actually a single press
        firstPressTime = currentTime;
        modeButtonPressCount = 1;
      }
    }
  }

  lastModeButton = buttonState;
}


void checkDoublePress()
{
  if (modeButtonPressCount == 1)
  {
    if (millis() - firstPressTime > DOUBLE_PRESS_TIME)
    {
      // It was a SINGLE PRESS

      if (mode == 1)
      {
        // RESET STOPWATCH
        stopwatchRunning = false;
        stopwatchElapsedMicros = 0;
      }

      modeButtonPressCount = 0;
    }
  }
}


void changeMode()
{
  if (mode == 0)
  {
    // Watch -> Stopwatch

    mode = 1;

    stopwatchRunning = false;
    stopwatchElapsedMicros = 0;
  }
  else
  {
    // Stopwatch -> Watch

    mode = 0;
  }

  lcd.clear();

  // Small mode message
  if (mode == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("REAL TIME");
    lcd.setCursor(0, 1);
    lcd.print("INDIA WATCH");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("STOPWATCH");
    lcd.setCursor(0, 1);
    lcd.print("READY");
  }

  delay(700);

  lcd.clear();
}

void showRealTime()
{
  unsigned long elapsedSeconds =
    (millis() - watchStartMillis) / 1000;

  unsigned long totalSeconds =
    START_HOUR * 3600UL +
    START_MINUTE * 60UL +
    START_SECOND +
    elapsedSeconds;

  // 24 hour clock
  totalSeconds = totalSeconds % 86400UL;

  int hours = totalSeconds / 3600;

  int minutes =
    (totalSeconds % 3600) / 60;

  int seconds =
    totalSeconds % 60;


  lcd.setCursor(0, 0);
  lcd.print("INDIA TIME      ");

  lcd.setCursor(0, 1);

  if (hours < 10)
    lcd.print("0");

  lcd.print(hours);
  lcd.print(":");

  if (minutes < 10)
    lcd.print("0");

  lcd.print(minutes);
  lcd.print(":");

  if (seconds < 10)
    lcd.print("0");

  lcd.print(seconds);

  lcd.print("       ");
}

void showStopwatch()
{
  unsigned long currentMicros;

  if (stopwatchRunning)
  {
    currentMicros =
      micros() - stopwatchStartMicros;
  }
  else
  {
    currentMicros =
      stopwatchElapsedMicros;
  }


  unsigned long milliseconds =
    currentMicros / 1000;


  unsigned long hours =
    milliseconds / 3600000UL;


  unsigned long minutes =
    (milliseconds % 3600000UL) / 60000UL;


  unsigned long seconds =
    (milliseconds % 60000UL) / 1000UL;


  unsigned long ms =
    milliseconds % 1000UL;


  lcd.setCursor(0, 0);
  lcd.print("STOPWATCH       ");


  lcd.setCursor(0, 1);

  if (hours < 10)
    lcd.print("0");

  lcd.print(hours);

  lcd.print(":");

  if (minutes < 10)
    lcd.print("0");

  lcd.print(minutes);

  lcd.print(":");

  if (seconds < 10)
    lcd.print("0");

  lcd.print(seconds);

  lcd.print(".");

  if (ms < 100)
    lcd.print("0");

  if (ms < 10)
    lcd.print("0");

  lcd.print(ms);
}
