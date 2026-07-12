#include <LedControl.h>

LedControl lc = LedControl(11, 13, 10, 1);


const int JOY_X = A0;
const int JOY_Y = A1;
const int JOY_SW = 5;

const int LEFT_THRESHOLD  = 400;
const int RIGHT_THRESHOLD = 600;
const int UP_THRESHOLD    = 400;
const int DOWN_THRESHOLD  = 600;

// position on 8x8 matrix (col: 0..7, row: 0..7)
int xPos = 3; // column (left-right)
int yPos = 3; // row (top-bottom)


unsigned long lastMove = 0;
const unsigned long moveDelay = 150; 

void setup() {
  pinMode(JOY_SW, INPUT_PULLUP); 
  lc.shutdown(0, false);    
  lc.setIntensity(0, 8);   
  lc.clearDisplay(0);       
  

  drawDot(xPos, yPos);
}

void loop() {
  unsigned long now = millis();
  
  int xVal = analogRead(JOY_X); 
  int yVal = analogRead(JOY_Y);
  int swVal = digitalRead(JOY_SW); 
  

  if (swVal == LOW) {
    xPos = 3;
    yPos = 3;
    lc.clearDisplay(0);
    drawDot(xPos, yPos);
    delay(200); 
    lastMove = now; 
    return;
  }

  if (now - lastMove >= moveDelay) {
    bool moved = false;

    if (xVal < LEFT_THRESHOLD) {
      // left
      if (xPos > 0) {
        xPos--;
        moved = true;
      }
    } else if (xVal > RIGHT_THRESHOLD) {
      if (xPos < 7) {
        xPos++;
        moved = true;
      }
    }

    if (yVal < UP_THRESHOLD) {
     
      if (yPos > 0) {
        yPos--;
        moved = true;
      }
    } else if (yVal > DOWN_THRESHOLD) {
      if (yPos < 7) {
        yPos++;
        moved = true;
      }
    }

    if (moved) {
      lc.clearDisplay(0);
      drawDot(xPos, yPos);
      lastMove = now;
    }
  }


  delay(10);
}


void drawDot(int x, int y) {
  lc.setLed(0, y, x, true);
}

