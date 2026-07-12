#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <avr/pgmspace.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define BUTTON_YES_PIN 2
#define BUTTON_NO_PIN 13
#define BUZZER_PIN 12

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum Emotion {
HAPPY,
ANGRY,
SAD,
SUSPICIOUS,
SURPRISE,
SLEEP,
TALKING
};

enum SystemState {
STATE_IDLE,
STATE_SCANNING,
STATE_RESULT
};

SystemState currentState = STATE_IDLE;
Emotion currentEmotion = HAPPY;

int pupilXOffset = 0;
int pupilYOffset = 0;
bool isBlinking = false;
unsigned long lastBlinkTime = 0;
unsigned long blinkDuration = 150;
unsigned long nextBlinkInterval = 3000;

unsigned long lastIdleAnimTime = 0;
unsigned long nextIdleAnimInterval = 2500;

unsigned long talkingAnimTime = 0;
bool mouthOpen = false;

char statusBuffer[24] = "";
char resultTitleBuffer[8] = "";
char resultMessageBuffer[24] = "";
int progressPercent = -1;

const char msg_ready[] PROGMEM = "LIE DETECTOR READY";
const char msg_analyzing[] PROGMEM = "Analyzing...";
const char msg_checking[] PROGMEM = "Checking...";
const char msg_dchecking[] PROGMEM = "Double Checking...";
const char msg_run_ai[] PROGMEM = "Running AI...";
const char msg_nasa_conn[] PROGMEM = "Connecting NASA...";
const char msg_nasa_says[] PROGMEM = "NASA says:";
const char msg_no_idea[] PROGMEM = "No Idea";
const char msg_fbi_call[] PROGMEM = "Calling FBI...";
const char msg_fbi_busy[] PROGMEM = "FBI Busy...";
const char msg_mom_try[] PROGMEM = "Trying Mom...";
const char msg_mom_says[] PROGMEM = "Mom Says:";
const char msg_suspicious[] PROGMEM = "Looks Suspicious";
const char msg_almost[] PROGMEM = "Almost Done...";
const char msg_oops[] PROGMEM = "Oops...";
const char msg_forgot[] PROGMEM = "Forgot Something...";
const char msg_reset[] PROGMEM = "Resetting...";
const char msg_scan2[] PROGMEM = "Second Scan";
const char msg_scan_fast[] PROGMEM = "Scanning Fast...";
const char msg_kidding[] PROGMEM = "Just Kidding...";

const char res_truth_t[] PROGMEM = "TRUTH";
const char res_liar_t[] PROGMEM = "LIAR";
const char res_maybe_t[] PROGMEM = "MAYBE";
const char res_error_t[] PROGMEM = "ERROR";

const char res_truth_m[] PROGMEM = "Even NASA Agrees!";
const char res_liar_m[] PROGMEM = "Your Mom Knew!";
const char res_maybe_m[] PROGMEM = "Robot Confused!";
const char res_error_m[] PROGMEM = "System Crashed!";

void setStatusText(const char* flashStr) {
strcpy_P(statusBuffer, flashStr);
}

void playBootSound() {
tone(BUZZER_PIN, 440, 100);
delay(120);
tone(BUZZER_PIN, 554, 100);
delay(120);
tone(BUZZER_PIN, 659, 100);
delay(120);
tone(BUZZER_PIN, 880, 200);
delay(220);
}

void playScanBeep() {
tone(BUZZER_PIN, 1200, 15);
}

void playFunnyBeep() {
tone(BUZZER_PIN, 800, 50);
delay(70);
tone(BUZZER_PIN, 400, 50);
delay(70);
}

void playErrorSound() {
tone(BUZZER_PIN, 220, 300);
delay(350);
}

void playResultSound() {
if (strcmp(resultTitleBuffer, "TRUTH") == 0) {
tone(BUZZER_PIN, 523, 150);
delay(180);
tone(BUZZER_PIN, 659, 150);
delay(180);
tone(BUZZER_PIN, 784, 150);
delay(180);
tone(BUZZER_PIN, 1047, 400);
delay(420);
} else if (strcmp(resultTitleBuffer, "LIAR") == 0) {
tone(BUZZER_PIN, 392, 200);
delay(250);
tone(BUZZER_PIN, 311, 200);
delay(250);
tone(BUZZER_PIN, 247, 400);
delay(450);
} else {
tone(BUZZER_PIN, 440, 150);
delay(200);
tone(BUZZER_PIN, 440, 150);
delay(200);
}
}

void drawEyes() {
int leftEyeX = 35;
int rightEyeX = 93;
int eyeY = 18;
int w = 24;
int h = 18;

if (isBlinking || currentEmotion == SLEEP) {
display.drawLine(leftEyeX - (w / 2), eyeY, leftEyeX + (w / 2), eyeY, SH110X_WHITE);
display.drawLine(rightEyeX - (w / 2), eyeY, rightEyeX + (w / 2), eyeY, SH110X_WHITE);
return;
}

if (currentEmotion == SUSPICIOUS) {
display.drawRoundRect(leftEyeX - (w / 2), eyeY - (h / 2), w, h, 6, SH110X_WHITE);
display.drawRoundRect(rightEyeX - (w / 2), eyeY - (h / 2), w, h, 6, SH110X_WHITE);
display.fillRect(leftEyeX - (w / 2), eyeY - (h / 2), w, h / 2, SH110X_BLACK);
display.fillRect(rightEyeX - (w / 2), eyeY - (h / 2), w, h / 2, SH110X_BLACK);
display.drawLine(leftEyeX - (w / 2), eyeY, leftEyeX + (w / 2), eyeY, SH110X_WHITE);
display.drawLine(rightEyeX - (w / 2), eyeY, rightEyeX + (w / 2), eyeY, SH110X_WHITE);

int pX = leftEyeX + pupilXOffset;
int pY = eyeY + 2 + pupilYOffset;
if (pY < eyeY + 1) pY = eyeY + 1;
display.fillCircle(pX, pY, 3, SH110X_WHITE);

pX = rightEyeX + pupilXOffset;
display.fillCircle(pX, pY, 3, SH110X_WHITE);
return;
}

display.drawRoundRect(leftEyeX - (w / 2), eyeY - (h / 2), w, h, 6, SH110X_WHITE);
display.drawRoundRect(rightEyeX - (w / 2), eyeY - (h / 2), w, h, 6, SH110X_WHITE);

int pLeftX = leftEyeX + pupilXOffset;
int pRightX = rightEyeX + pupilXOffset;
int pY = eyeY + pupilYOffset;

display.fillCircle(pLeftX, pY, 4, SH110X_WHITE);
display.fillCircle(pRightX, pY, 4, SH110X_WHITE);
}

void drawEyebrows() {
int leftEyeX = 35;
int rightEyeX = 93;
int eyeY = 18;
int hw = 12;

switch (currentEmotion) {
case ANGRY:
display.drawLine(leftEyeX - hw, eyeY - 14, leftEyeX + hw - 2, eyeY - 8, SH110X_WHITE);
display.drawLine(rightEyeX - hw + 2, eyeY - 8, rightEyeX + hw, eyeY - 14, SH110X_WHITE);
break;
case SUSPICIOUS:
display.drawLine(leftEyeX - hw, eyeY - 10, leftEyeX + hw, eyeY - 12, SH110X_WHITE);
display.drawLine(rightEyeX - hw, eyeY - 12, rightEyeX + hw, eyeY - 8, SH110X_WHITE);
break;
case SURPRISE:
display.drawRoundRect(leftEyeX - hw, eyeY - 16, hw * 2, 3, 1, SH110X_WHITE);
display.drawRoundRect(rightEyeX - hw, eyeY - 16, hw * 2, 3, 1, SH110X_WHITE);
break;
case SAD:
display.drawLine(leftEyeX - hw, eyeY - 8, leftEyeX + hw - 2, eyeY - 14, SH110X_WHITE);
display.drawLine(rightEyeX - hw + 2, eyeY - 14, rightEyeX + hw, eyeY - 8, SH110X_WHITE);
break;
default:
display.drawLine(leftEyeX - hw, eyeY - 12, leftEyeX + hw, eyeY - 12, SH110X_WHITE);
display.drawLine(rightEyeX - hw, eyeY - 12, rightEyeX + hw, eyeY - 12, SH110X_WHITE);
break;
}
}

void drawMouth() {
int mouthX = 64;
int mouthY = 34;

if (currentEmotion == TALKING) {
if (mouthOpen) {
display.fillRoundRect(mouthX - 8, mouthY - 3, 16, 6, 3, SH110X_WHITE);
} else {
display.drawLine(mouthX - 10, mouthY, mouthX + 10, mouthY, SH110X_WHITE);
}
return;
}

switch (currentEmotion) {
case HAPPY:
display.drawRoundRect(mouthX - 12, mouthY - 4, 24, 8, 4, SH110X_WHITE);
display.fillRect(mouthX - 12, mouthY - 4, 24, 4, SH110X_BLACK);
display.drawLine(mouthX - 12, mouthY, mouthX + 12, mouthY, SH110X_WHITE);
break;
case ANGRY:
case SAD:
display.drawRoundRect(mouthX - 12, mouthY, 24, 8, 4, SH110X_WHITE);
display.fillRect(mouthX - 12, mouthY + 4, 24, 4, SH110X_BLACK);
display.drawLine(mouthX - 12, mouthY + 4, mouthX + 12, mouthY + 4, SH110X_WHITE);
break;
case SUSPICIOUS:
display.drawLine(mouthX - 8, mouthY + 2, mouthX + 8, mouthY - 2, SH110X_WHITE);
break;
case SURPRISE:
display.drawCircle(mouthX, mouthY, 5, SH110X_WHITE);
break;
case SLEEP:
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);
display.setCursor(mouthX - 3, mouthY - 3);
display.print("z");
break;
}
}

void drawProgressBar(int percent) {
int x = 14;
int y = 56;
int w = 100;
int h = 6;

display.drawRect(x, y, w, h, SH110X_WHITE);
int fillWidth = (percent * w) / 100;
display.fillRect(x + 1, y + 1, fillWidth, h - 2, SH110X_WHITE);
}

void render() {
display.clearDisplay();

drawEyes();
drawEyebrows();
drawMouth();

if (strlen(statusBuffer) > 0) {
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);
int16_t x1, y1;
uint16_t w, h;
display.getTextBounds(statusBuffer, 0, 0, &x1, &y1, &w, &h);
display.setCursor((SCREEN_WIDTH - w) / 2, 44);
display.print(statusBuffer);
}

if (progressPercent >= 0) {
drawProgressBar(progressPercent);
}

display.display();
}

void updateBlink() {
unsigned long currentMillis = millis();
if (!isBlinking) {
if (currentMillis - lastBlinkTime >= nextBlinkInterval) {
isBlinking = true;
lastBlinkTime = currentMillis;
}
} else {
if (currentMillis - lastBlinkTime >= blinkDuration) {
isBlinking = false;
lastBlinkTime = currentMillis;
nextBlinkInterval = random(2000, 6000);
}
}
}

void updateIdle() {
unsigned long currentMillis = millis();
if (currentMillis - lastIdleAnimTime >= nextIdleAnimInterval) {
lastIdleAnimTime = currentMillis;
nextIdleAnimInterval = random(1500, 4000);

int roll = random(0, 100);
if (roll < 40) {
  currentEmotion = HAPPY;
  pupilXOffset = 0;
  pupilYOffset = 0;
} else if (roll < 65) {
  currentEmotion = SUSPICIOUS;
  pupilXOffset = random(-4, 5);
  pupilYOffset = random(-2, 3);
  playFunnyBeep();
} else {
  currentEmotion = HAPPY;
  int lookDir = random(0, 4);
  if (lookDir == 0) { pupilXOffset = -5; pupilYOffset = 0; }
  else if (lookDir == 1) { pupilXOffset = 5; pupilYOffset = 0; }
  else if (lookDir == 2) { pupilXOffset = 0; pupilYOffset = -3; }
  else { pupilXOffset = 0; pupilYOffset = 3; }
}
}
}

void updateTalking() {
unsigned long currentMillis = millis();
if (currentMillis - talkingAnimTime >= 120) {
talkingAnimTime = currentMillis;
mouthOpen = !mouthOpen;
}
}

void runProgressSegment(const char* flashStr, Emotion emotion, int startPerc, int endPerc, int delayMs, bool sound) {
setStatusText(flashStr);
currentEmotion = emotion;
int steps = abs(endPerc - startPerc);
if (steps == 0) steps = 1;
int stepDelay = delayMs / steps;

int i = startPerc;
while (true) {
progressPercent = i;
updateBlink();
updateTalking();
render();
if (sound && i % 4 == 0) {
playScanBeep();
}
delay(stepDelay);
if (i == endPerc) break;
if (startPerc < endPerc) i++; else i--;
}
}

void showStaticMessage(const char* flashStr, Emotion emotion, int durationMs, int progressVal) {
setStatusText(flashStr);
currentEmotion = emotion;
progressPercent = progressVal;

unsigned long startTime = millis();
while (millis() - startTime < (unsigned long)durationMs) {
updateBlink();
updateTalking();
render();
delay(15);
}
}

void generateResult() {
int roll = random(0, 4);
if (roll == 0) {
strcpy_P(resultTitleBuffer, res_truth_t);
strcpy_P(resultMessageBuffer, res_truth_m);
currentEmotion = HAPPY;
} else if (roll == 1) {
strcpy_P(resultTitleBuffer, res_liar_t);
strcpy_P(resultMessageBuffer, res_liar_m);
currentEmotion = ANGRY;
} else if (roll == 2) {
strcpy_P(resultTitleBuffer, res_maybe_t);
strcpy_P(resultMessageBuffer, res_maybe_m);
currentEmotion = SUSPICIOUS;
} else {
strcpy_P(resultTitleBuffer, res_error_t);
strcpy_P(resultMessageBuffer, res_error_m);
currentEmotion = SAD;
}
}

void showResult() {
currentState = STATE_RESULT;
progressPercent = -1;
pupilXOffset = 0;
pupilYOffset = 0;

playResultSound();
strcpy(statusBuffer, resultMessageBuffer);

unsigned long displayStartTime = millis();
while (millis() - displayStartTime < 5000) {
updateBlink();
display.clearDisplay();

drawEyes();
drawEyebrows();
drawMouth();

display.setTextSize(2);
display.setTextColor(SH110X_WHITE);
int16_t x1, y1;
uint16_t w, h;
display.getTextBounds(resultTitleBuffer, 0, 0, &x1, &y1, &w, &h);
display.setCursor((SCREEN_WIDTH - w) / 2, 42);
display.print(resultTitleBuffer);

display.setTextSize(1);
display.getTextBounds(resultMessageBuffer, 0, 0, &x1, &y1, &w, &h);
display.setCursor((SCREEN_WIDTH - w) / 2, 56);
display.print(resultMessageBuffer);

display.display();
delay(15);
}

currentState = STATE_IDLE;
currentEmotion = HAPPY;
setStatusText(msg_ready);
lastIdleAnimTime = millis();
}

void startScan() {
currentState = STATE_SCANNING;
pupilXOffset = 0;
pupilYOffset = 0;

runProgressSegment(msg_analyzing, SUSPICIOUS, 0, 20, 1200, true);
showStaticMessage(msg_checking, SURPRISE, 800, 20);
runProgressSegment(msg_dchecking, ANGRY, 20, 45, 1500, true);
showStaticMessage(msg_run_ai, SUSPICIOUS, 1000, 45);
runProgressSegment(msg_nasa_conn, SURPRISE, 45, 70, 1800, true);

showStaticMessage(msg_nasa_says, HAPPY, 800, 70);
showStaticMessage(msg_no_idea, SAD, 1200, 70);
playFunnyBeep();

showStaticMessage(msg_fbi_call, ANGRY, 1000, 70);
showStaticMessage(msg_fbi_busy, SUSPICIOUS, 1200, 70);
playFunnyBeep();

showStaticMessage(msg_mom_try, HAPPY, 1000, 70);
showStaticMessage(msg_mom_says, SURPRISE, 800, 70);
showStaticMessage(msg_suspicious, SUSPICIOUS, 1500, 70);

runProgressSegment(msg_analyzing, ANGRY, 70, 99, 2000, true);
showStaticMessage(msg_almost, SURPRISE, 1200, 99);

showStaticMessage(msg_oops, SAD, 1000, 99);
playErrorSound();
showStaticMessage(msg_forgot, SUSPICIOUS, 1500, 99);

runProgressSegment(msg_reset, SAD, 99, 0, 500, false);
showStaticMessage(msg_scan2, HAPPY, 1000, 0);

runProgressSegment(msg_scan_fast, SURPRISE, 0, 100, 1000, true);
showStaticMessage(msg_kidding, HAPPY, 1200, 100);

generateResult();
showResult();
}

void setup() {
pinMode(BUTTON_YES_PIN, INPUT_PULLUP);
pinMode(BUTTON_NO_PIN, INPUT_PULLUP);
pinMode(BUZZER_PIN, OUTPUT);

display.begin(SCREEN_ADDRESS, true);
display.clearDisplay();
display.display();

playBootSound();
setStatusText(msg_ready);

lastBlinkTime = millis();
lastIdleAnimTime = millis();
}

void loop() {
updateBlink();

if (currentState == STATE_IDLE) {
updateIdle();
if (digitalRead(BUTTON_YES_PIN) == LOW || digitalRead(BUTTON_NO_PIN) == LOW) {
delay(50);
if (digitalRead(BUTTON_YES_PIN) == LOW || digitalRead(BUTTON_NO_PIN) == LOW) {
startScan();
}
}
} else if (currentState == STATE_SCANNING) {
updateTalking();
}

render();
delay(15);
}