#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <math.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

// ---------------- Pins ----------------

#define BTN_YES 2
#define BTN_NO 13
#define BUZZER 12

// ---------------- Variables ----------------

int pupilX = 0;
int pupilY = 0;

bool blink = false;

unsigned long lastBlink = 0;
unsigned long lastMove = 0;

int eyeOpen = 100;  // 100 = fully open
bool winkLeft = false;
bool winkRight = false;

bool talking = false;
byte mouthFrame = 0;
unsigned long lastTalk = 0;

bool scanning = false;
int scanProgress = 0;
unsigned long lastScan = 0;

String finalResult = "";
int confidence = 0;

String statusText = "Ready...";

// ========================================
// Draw Left / Right Eye
// ========================================

void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) {
  for (int yy = -ry; yy <= ry; yy++) {
    float xr = rx * sqrt(1.0 - ((float)(yy * yy) / (ry * ry)));

    display.drawFastHLine(
      cx - (int)xr,
      cy + yy,
      (int)xr * 2,
      color);
  }
}

void drawEye(int x, int y, bool wink) {
  if (blink || wink) {
    display.drawFastHLine(x - 18, y, 36, SH110X_WHITE);
    return;
  }

  fillEllipse(x, y, 18, 11, SH110X_WHITE);

  display.drawCircle(x, y, 18, SH110X_WHITE);

  display.fillCircle(
    x + pupilX,
    y + pupilY,
    5,
    SH110X_BLACK);

  display.fillCircle(
    x + pupilX - 2,
    y + pupilY - 2,
    1,
    SH110X_WHITE);

  // Upper eyelid

  int lidHeight = map(eyeOpen, 0, 100, 11, 0);

  display.fillRect(
    x - 18,
    y - 11,
    36,
    lidHeight,
    SH110X_BLACK);
}

void drawNose() {
  display.drawLine(64, 34, 62, 38, SH110X_WHITE);
  display.drawLine(62, 38, 66, 38, SH110X_WHITE);
}

// ========================================

enum FaceMode
{
  NORMAL,
  HAPPY,
  ANGRY,
  SAD,
  SUSPICIOUS,
  SURPRISED
};

FaceMode faceMode = NORMAL;

void drawEyebrows() {
  switch (faceMode) {
    case NORMAL:

      display.fillRoundRect(16, 6, 30, 3, 2, SH110X_WHITE);
      display.fillRoundRect(82, 6, 30, 3, 2, SH110X_WHITE);

      break;

    case HAPPY:

      display.drawLine(16, 10, 46, 5, SH110X_WHITE);
      display.drawLine(82, 5, 112, 10, SH110X_WHITE);

      break;

    case ANGRY:

      display.drawLine(16, 3, 46, 11, SH110X_WHITE);
      display.drawLine(82, 11, 112, 3, SH110X_WHITE);

      break;

    case SAD:

      display.drawLine(16, 10, 46, 15, SH110X_WHITE);
      display.drawLine(82, 15, 112, 10, SH110X_WHITE);

      break;

    case SUSPICIOUS:

      display.fillRoundRect(16, 5, 30, 3, 2, SH110X_WHITE);

      display.drawLine(82, 5, 112, 8, SH110X_WHITE);

      break;
  }
}

// ========================================

void drawMouth() {

  if (talking) {

    switch (mouthFrame) {

      case 0:

        display.drawLine(52, 38, 76, 50, SH110X_WHITE);

        break;

      case 1:

        display.drawRoundRect(56, 36, 16, 6, 2, SH110X_WHITE);

        break;

      case 2:

        display.fillRoundRect(56, 34, 16, 10, 3, SH110X_WHITE);

        break;

      case 3:

        display.drawCircle(64, 39, 5, SH110X_WHITE);

        break;
    }

    return;
  }

  switch (faceMode) {

    case HAPPY:

      display.drawLine(50, 48, 56, 53, SH110X_WHITE);
      display.drawLine(56, 53, 72, 53, SH110X_WHITE);
      display.drawLine(72, 53, 78, 48, SH110X_WHITE);

      break;

    case SAD:

      display.drawLine(50, 54, 56, 49, SH110X_WHITE);
      display.drawLine(56, 49, 72, 49, SH110X_WHITE);
      display.drawLine(72, 49, 78, 54, SH110X_WHITE);

      break;

    case ANGRY:

      display.drawLine(50, 53, 64, 49, SH110X_WHITE);
      display.drawLine(64, 49, 78, 53, SH110X_WHITE);

      break;

    default:

      display.drawLine(52, 50, 76, 50, SH110X_WHITE);

      break;
  }
}

void updateTalking() {

  if (!talking)
    return;

  if (millis() - lastTalk < 90)
    return;

  lastTalk = millis();

  mouthFrame++;

  if (mouthFrame > 3)
    mouthFrame = 0;
}


void drawProgressBar()
{
    display.drawRoundRect(14,56,100,6,2,SH110X_WHITE);

    int width = map(scanProgress,0,100,0,96);

    display.fillRoundRect(16,58,width,2,1,SH110X_WHITE);
}

void startScan()
{
  scanning = true;

  // ---------- FIRST SCAN ----------

  faceMode = SUSPICIOUS;

  for (int i = 0; i <= 100; i++)
  {
    scanProgress = i;

    // Eye movement
    switch ((i / 10) % 5)
    {
      case 0: pupilX = -5; pupilY = 0; break;
      case 1: pupilX = 5;  pupilY = 0; break;
      case 2: pupilX = 0;  pupilY = -3; break;
      case 3: pupilX = 0;  pupilY = 3; break;
      default: pupilX = 0; pupilY = 0; break;
    }

    // Status Messages

    if(i < 20)
      statusText = "Analyzing...";

    else if(i < 40)
      statusText = "Checking...";

    else if(i < 60)
    {
      statusText = "Double Check...";
      faceMode = SUSPICIOUS;
    }

    else if(i < 80)
    {
      statusText = "Running AI...";
      faceMode = NORMAL;
    }

    else
    {
      statusText = "Connecting NASA";
      faceMode = HAPPY;
    }

    render();

    tone(BUZZER,700+i*5,5);

    delay(18);
  }

  // ---------- NASA ----------

  statusText="NASA: No Idea";
  faceMode=HAPPY;
  render();

  tone(BUZZER,1600,200);

  delay(1200);

  // ---------- FBI ----------

  statusText="Calling FBI...";
  faceMode=SUSPICIOUS;
  render();

  delay(1000);

  statusText="FBI Busy...";
  faceMode=SAD;
  render();

  delay(1000);

  // ---------- MOM ----------

  statusText="Trying Mom...";
  faceMode=NORMAL;
  render();

  delay(1200);

  statusText="Mom: Suspicious";
  faceMode=HAPPY;
  render();

  delay(1400);

  // ---------- OOPS ----------

  statusText="Oops...";
  faceMode=ANGRY;
  render();

  tone(BUZZER,350,400);

  delay(1000);

  statusText="Forgot Something!";
  render();

  delay(1500);

  // ---------- SECOND SCAN ----------

  faceMode=ANGRY;

  for(int i=0;i<=100;i++)
  {
    scanProgress=i;

    pupilX=random(-5,6);
    pupilY=random(-3,4);

    statusText="Analyzing Again...";

    render();

    tone(BUZZER,900+i*3,4);

    delay(10);
  }

  // ---------- TROLL ----------

  statusText="Just Kidding...";
  faceMode=HAPPY;
  render();

  tone(BUZZER,1800,150);

  delay(1500);

  // ---------- RESULT ----------

  scanning=false;

  statusText="";

  generateResult();

  showResult();

  faceMode=NORMAL;

  pupilX=0;
  pupilY=0;

  render();
}

void generateResult() {
  int r = random(100);

  confidence = random(90, 100);

  if (r < 40) {
    finalResult = "TRUTH";
    faceMode = HAPPY;
  } else if (r < 80) {
    finalResult = "LIAR";
    faceMode = ANGRY;
  } else if (r < 90) {
    finalResult = "MAYBE";
    faceMode = SUSPICIOUS;
  } else {
    finalResult = "ERROR";
    faceMode = SAD;
  }
}

void showResult()
{
  String msg = "";

  if(finalResult == "TRUTH")
  {
    msg = "Even NASA Agrees!";
    faceMode = HAPPY;
  }
  else if(finalResult == "LIAR")
  {
    msg = "Your Mom Knew!";
    faceMode = ANGRY;
  }
  else if(finalResult == "MAYBE")
  {
    msg = "Robot Confused!";
    faceMode = SUSPICIOUS;
  }
  else
  {
    msg = "System Crashed!";
    faceMode = SAD;
  }

  // -------- Result --------

  display.clearDisplay();

  drawEyebrows();
  drawEye(32,30,winkLeft);
  drawEye(96,30,winkRight);
  drawMouth();

  display.setTextSize(2);

  int16_t x1,y1;
  uint16_t w,h;

  display.getTextBounds(finalResult,0,0,&x1,&y1,&w,&h);

  display.setCursor((128-w)/2,50);
  display.print(finalResult);

  display.display();

  if(finalResult=="TRUTH")
  {
    tone(BUZZER,1500,120);
    delay(150);
    tone(BUZZER,1900,180);
  }
  else if(finalResult=="LIAR")
  {
    tone(BUZZER,450,350);
  }
  else
  {
    tone(BUZZER,900,250);
  }

  delay(1800);

  // -------- Funny Message --------

  display.clearDisplay();

  drawEyebrows();
  drawEye(32,30,winkLeft);
  drawEye(96,30,winkRight);
  drawMouth();

  display.setTextSize(1);

  display.setCursor(8,46);
  display.print(msg);

  display.display();

  delay(2200);

  // -------- Back To Idle --------

  statusText="Ready";

  faceMode=NORMAL;

  pupilX=0;
  pupilY=0;

  render();
}
// ========================================

void render()
{
  display.clearDisplay();

  drawEyebrows();

  drawEye(32,20,winkLeft);
  drawEye(96,20,winkRight);

  drawMouth();

  // Divider
  display.drawFastHLine(0,44,128,SH110X_WHITE);

  display.setTextSize(1);

  int16_t x1,y1;
  uint16_t w,h;

  display.getTextBounds(statusText,0,0,&x1,&y1,&w,&h);

  display.setCursor((128-w)/2,47);
  display.print(statusText);

  if(scanning)
  {
      drawProgressBar();
  }

  display.display();
}

// ========================================

void blinkEyes() {
  blink = true;

  render();

  delay(80);

  blink = false;

  render();
}


void leftWink() {

  winkLeft = true;

  render();

  delay(180);

  winkLeft = false;
}

void rightWink() {

  winkRight = true;

  render();

  delay(180);

  winkRight = false;
}
// ========================================

void randomEyes() {
  if (millis() - lastMove < 1200)
    return;

  lastMove = millis();

  switch (random(5)) {
    case 0:
      pupilX = -5;
      pupilY = 0;
      break;

    case 1:
      pupilX = 5;
      pupilY = 0;
      break;

    case 2:
      pupilX = 0;
      pupilY = -3;
      break;

    case 3:
      pupilX = 0;
      pupilY = 3;
      break;

    default:
      pupilX = 0;
      pupilY = 0;
      break;
  }
}

// ========================================

void autoBlink() {

  if (millis() - lastBlink < 3500)
    return;

  lastBlink = millis();

  eyeOpen = 0;

  render();

  delay(80);

  eyeOpen = 100;
}

// ========================================

void setup() {

  pinMode(BTN_YES, INPUT_PULLUP);

  pinMode(BTN_NO, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  Wire.begin();

  display.begin(0x3C, true);

  randomSeed(analogRead(A0));

  tone(BUZZER, 1200, 100);
}

// ========================================

void loop() {
  updateTalking();
  randomEyes();

  autoBlink();

  render();

  if (!digitalRead(BTN_YES)) {
    while (!digitalRead(BTN_YES))
      ;
    talking = true;

    for (int i = 0; i < 20; i++) {
      updateTalking();

      render();

      tone(BUZZER, 1200 + random(400), 25);

      delay(70);
    }

    talking = false;
    startScan();
    rightWink();

    faceMode = (FaceMode)(faceMode + 1);

    if (faceMode > SUSPICIOUS)
      faceMode = NORMAL;

    tone(BUZZER, 1500, 40);
  }

  if (!digitalRead(BTN_NO)) {
    while (!digitalRead(BTN_NO))
      ;
    leftWink();

    if (faceMode == NORMAL)
      faceMode = SUSPICIOUS;
    else
      faceMode = (FaceMode)(faceMode - 1);

    tone(BUZZER, 900, 40);
  }
}