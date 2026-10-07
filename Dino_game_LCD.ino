#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BUTTON 2
#define BUZZER 8
byte dino[8] = {
  B00111,
  B00101,
  B00111,
  B10110,
  B11111,
  B01110,
  B01010,
  B01010
};
byte cactus[8] = {
  B00100,
  B00100,
  B10101,
  B10101,
  B11111,
  B00100,
  B00100,
  B00100
};
int cactusPos = 15;
int score = 0;
bool jumping = false;
bool gameOver = false;
unsigned long lastMove = 0;
unsigned long jumpStart = 0;
int gameSpeed = 400;
void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
  showStart();
}
void loop() {
  if (gameOver) {
    if (digitalRead(BUTTON) == LOW) {
      delay(250);
      while (digitalRead(BUTTON) == LOW) {
        delay(10);
      }
      restartGame();
    }
    return;
  }
  if (digitalRead(BUTTON) == LOW && !jumping) {
    jumping = true;
    jumpStart = millis();
    tone(BUZZER, 1000, 80);
    delay(100);
  }
  if (jumping && millis() - jumpStart >= 650) {
    jumping = false;
  }
  if (millis() - lastMove >= gameSpeed) {
    lastMove = millis();
    cactusPos--;
    if (cactusPos == 1 && !jumping) {
      gameOver = true;
      tone(BUZZER, 200, 500);
      showGameOver();
      return;
    }
    if (cactusPos == 0) {
      score++;
      tone(BUZZER, 1500, 60);
      increaseSpeed();
      cactusPos = 15;
    }
    drawGame();
  }
}
void drawGame() {
  lcd.clear();
  lcd.setCursor(10, 0);
  lcd.print("S:");
  if (score < 10) {
    lcd.print("0");
  }
  lcd.print(score);
  if (jumping) {
    lcd.setCursor(1, 0);
    lcd.write(byte(0));

  } else {
    lcd.setCursor(1, 1);
    lcd.write(byte(0));
  }
  if (cactusPos >= 0 && cactusPos < 16) {
    lcd.setCursor(cactusPos, 1);
    lcd.write(byte(1));
  }
}

void increaseSpeed() {
  if (score == 5) {
    gameSpeed = 330;
  }
  else if (score == 10) {
    gameSpeed = 280;
  }
  else if (score == 15) {
    gameSpeed = 230;
  }
  else if (score == 20) {
    gameSpeed = 190;
  }
  else if (score == 25) {
    gameSpeed = 160;
  }
  else if (score == 30) {
    gameSpeed = 140;
  }
}

void showStart() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("DINO JUMP!");
  lcd.setCursor(1, 1);
  lcd.print("Press Button");
  while (digitalRead(BUTTON) == HIGH) {
    delay(10);
  }

  while (digitalRead(BUTTON) == LOW) {
    delay(10);
  }
  delay(300);

  lcd.clear();
  lastMove = millis();
}
void showGameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME OVER!");
  lcd.setCursor(11, 0);
  lcd.print("S:");
  if (score < 10) {
    lcd.print("0");
  }
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Press Restart");
}
void restartGame() {
  score = 0;
  cactusPos = 15;
  gameSpeed = 400;
  jumping = false;
  gameOver = false;
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("READY!");
  delay(1000);
  lcd.clear();
  lastMove = millis();
}