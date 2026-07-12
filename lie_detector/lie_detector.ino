#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128,64,&Wire,-1);

#define BTN_YES 2
#define BTN_NO 13
#define BUZZER 12

enum FaceExpression
{
  NORMAL,
  HAPPY,
  ANGRY,
  SAD,
  SUSPICIOUS,
  COOL,
  SURPRISED,
  SLEEP,
  LAUGH,
  SCAN
};

FaceExpression currentFace = ANGRY;

int pupilX = 0;
int pupilY = 0;

bool blink = false;

unsigned long lastBlink = 0;

bool mouthOpen = false;

// ---------- Lie Detector ----------
bool detecting = false;
int detectProgress = 0;

String verdict = "";

const char* lieResults[] =
{
  "TRUTH",
  "LIAR",
  "MAYBE",
  "ERROR",
  "HMM..."
};

const char* funnyResults[] =
{
  "99% Honest",
  "98% Liar",
  "Ask Again",
  "Robot Confused",
  "Nice Try!"
};

// Animation
int targetPupilX = 0;
int targetPupilY = 0;

unsigned long lastIdle = 0;
byte idleMode = 0;

bool talking = false;
bool sleeping = false;


void updateEyes()
{
  if (pupilX < targetPupilX) pupilX++;
  if (pupilX > targetPupilX) pupilX--;

  if (pupilY < targetPupilY) pupilY++;
  if (pupilY > targetPupilY) pupilY--;
}


void idleAnimation()
{
  if (millis() - lastIdle < 1500)
    return;

  lastIdle = millis();

  idleMode = random(0, 6);

  switch (idleMode)
  {
    case 0:
      targetPupilX = -5;
      targetPupilY = 0;
      break;

    case 1:
      targetPupilX = 5;
      targetPupilY = 0;
      break;

    case 2:
      targetPupilX = 0;
      targetPupilY = -3;
      break;

    case 3:
      targetPupilX = 0;
      targetPupilY = 3;
      break;

    default:
      targetPupilX = 0;
      targetPupilY = 0;
      break;
  }

  if (random(100) < 30)
  {
    blinkEyes();
  }
}


void drawProgressBar(int value)
{
  display.drawRoundRect(14,54,100,8,4,SH110X_WHITE);

  display.fillRoundRect(
      16,
      56,
      value,
      4,
      2,
      SH110X_WHITE);
}

void analyzing()
{

  display.clearDisplay();

  drawFace(SCAN);

  display.setTextSize(1);

  display.setCursor(28,42);
  display.print("Analyzing...");

  drawProgressBar(detectProgress);

  display.display();

}

void fakeScan()
{
  currentFace = SCAN;

  for(int p=0;p<=100;p+=2)
  {
    pupilX = map(p,0,100,-5,5);

    display.clearDisplay();

    drawFace(currentFace);

    display.setTextSize(1);

    display.setCursor(18,42);
    display.print("Analyzing...");

    display.drawRoundRect(12,54,104,8,3,SH110X_WHITE);

    display.fillRoundRect(
      14,
      56,
      p,
      4,
      2,
      SH110X_WHITE);

    display.display();

    tone(BUZZER,900+p*4,8);

    delay(35);
  }

  pupilX=0;
}

void showVerdict()
{

  display.clearDisplay();

  drawFace(random(0,5));

  display.setCursor(24,42);

  display.setTextSize(2);

  display.print(lieResults[random(0,5)]);

  display.setTextSize(1);

  display.setCursor(18,58);

  display.print(funnyResults[random(0,5)]);

  display.display();

  tone(BUZZER,1500,200);

  delay(2500);

}


void talkAnimation()
{
  for (byte i = 0; i < 6; i++)
  {
    mouthOpen = !mouthOpen;

    display.fillRect(45, 44, 40, 18, SH110X_BLACK);

    if (mouthOpen)
    {
      display.fillRoundRect(56, 47, 16, 10, 5, SH110X_WHITE);
    }
    else
    {
      drawMouth(currentFace);
    }

    display.display();

    tone(BUZZER, 1200 + i * 80, 40);

    delay(90);
  }
}

void surpriseAnimation()
{
  display.clearDisplay();

  display.drawCircle(32, 28, 14, SH110X_WHITE);
  display.drawCircle(96, 28, 14, SH110X_WHITE);

  display.fillCircle(32, 28, 5, SH110X_WHITE);
  display.fillCircle(96, 28, 5, SH110X_WHITE);

  display.drawCircle(64, 50, 6, SH110X_WHITE);

  display.display();

  tone(BUZZER, 1800, 150);

  delay(400);
}

void laughAnimation()
{
  for (byte i = 0; i < 4; i++)
  {
    blink = !blink;

    drawFace(LAUGH);

    tone(BUZZER, 900 + i * 150, 60);

    delay(100);
  }

  blink = false;
}

void scanAnimation()
{
  currentFace = SCAN;

  for (byte i = 0; i < 3; i++)
  {
    smoothLook(-5, 0);
    tone(BUZZER, 1000, 40);

    smoothLook(5, 0);
    tone(BUZZER, 1300, 40);
  }

  smoothLook(0, 0);

  currentFace = NORMAL;
}


void drawEye(int x, int y)
{
  if (blink)
  {
    // Eyelid
    display.fillRoundRect(x-15, y-2, 30, 4, 2, SH110X_WHITE);
    return;
  }

  // White Eye
  display.fillRoundRect(x-15, y-12, 30, 24, 12, SH110X_WHITE);

  // Black Border
  display.drawRoundRect(x-15, y-12, 30, 24, 12, SH110X_BLACK);

  // Pupil
  display.fillCircle(x+pupilX, y+pupilY, 5, SH110X_BLACK);

  // Eye Shine
  display.drawPixel(x+pupilX-2,y+pupilY-2,SH110X_WHITE);
  display.drawPixel(x+pupilX-1,y+pupilY-3,SH110X_WHITE);
}

void smoothLook(int targetX,int targetY)
{
  while(pupilX!=targetX || pupilY!=targetY)
  {

    if(pupilX<targetX) pupilX++;
    if(pupilX>targetX) pupilX--;

    if(pupilY<targetY) pupilY++;
    if(pupilY>targetY) pupilY--;

    drawFace(currentFace);

    delay(25);

  }
}

void drawEyebrows(FaceExpression mood)
{

  switch(mood)
  {

    case NORMAL:

      display.drawLine(18,8,42,8,SH110X_WHITE);
      display.drawLine(86,8,110,8,SH110X_WHITE);

    break;

    case HAPPY:

      display.drawLine(18,12,42,8,SH110X_WHITE);
      display.drawLine(86,8,110,12,SH110X_WHITE);

    break;

    case ANGRY:

      display.drawLine(18,6,42,12,SH110X_WHITE);
      display.drawLine(86,12,110,6,SH110X_WHITE);

    break;

    case SAD:

      display.drawLine(18,10,42,14,SH110X_WHITE);
      display.drawLine(86,14,110,10,SH110X_WHITE);

    break;

    case SUSPICIOUS:

      display.drawLine(18,8,42,5,SH110X_WHITE);
      display.drawLine(86,5,110,8,SH110X_WHITE);

    break;

    case COOL:

    display.drawLine(18,10,42,10,SH110X_WHITE);
    display.drawLine(86,10,110,10,SH110X_WHITE);

    break;
  }

}

void drawMouth(FaceExpression mood)
{
  if (talking)
  {
    display.fillRoundRect(56, 47, 16, 10, 4, SH110X_WHITE);
    return;
  }

  switch (mood)
  {
    case NORMAL:
      display.drawLine(52,53,76,53,SH110X_WHITE);
      break;

    case HAPPY:
      display.drawLine(50,50,56,54,SH110X_WHITE);
      display.drawLine(56,54,72,54,SH110X_WHITE);
      display.drawLine(72,54,78,50,SH110X_WHITE);
      break;

    case ANGRY:
      display.drawLine(50,56,56,50,SH110X_WHITE);
      display.drawLine(56,50,72,50,SH110X_WHITE);
      display.drawLine(72,50,78,56,SH110X_WHITE);
      break;

    case SAD:
      display.drawLine(50,54,56,50,SH110X_WHITE);
      display.drawLine(56,50,72,50,SH110X_WHITE);
      display.drawLine(72,50,78,54,SH110X_WHITE);
      break;

    default:
      display.drawLine(52,53,76,53,SH110X_WHITE);
      break;
  }
}

void drawFace(FaceExpression mood)
{
  drawEyebrows(mood);

  drawEye(32,28);
  drawEye(96,28);

  drawMouth(mood);
}



void lookCenter() {
  pupilX = 0;
  pupilY = 0;
}

void lookLeft() {
  pupilX = -4;
  pupilY = 0;
}

void lookRight() {
  pupilX = 4;
  pupilY = 0;
}

void lookUp() {
  pupilX = 0;
  pupilY = -3;
}

void lookDown() {
  pupilX = 0;
  pupilY = 3;
}


void blinkEyes()
{
  blink = true;
  drawFace(currentFace);
  delay(120);

  blink = false;
  drawFace(currentFace);
}

void demoEyes()
{

  smoothLook(0,0);

  delay(300);

  smoothLook(-5,0);

  delay(250);

  smoothLook(5,0);

  delay(250);

  smoothLook(0,-3);

  delay(250);

  smoothLook(0,3);

  delay(250);

  smoothLook(0,0);

  blinkEyes();

}

void setup()
{

  pinMode(BTN_YES,INPUT_PULLUP);
  pinMode(BTN_NO,INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);

  display.begin(0x3C,true);

}

void loop()
{

  updateEyes();
  idleAnimation();
  display.clearDisplay();

  demoEyes();

  drawFace(currentFace);

  display.display();

  if(!digitalRead(BTN_YES))
  {
    while(!digitalRead(BTN_YES));

    fakeScan();

    showVerdict();
    display.clearDisplay();

drawFace(currentFace);

display.display();

  if(!digitalRead(BTN_NO))
  {
    while(!digitalRead(BTN_NO));

    surpriseAnimation();

    laughAnimation();
    
  }
}
}