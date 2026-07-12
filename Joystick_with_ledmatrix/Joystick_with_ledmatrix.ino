#include <LedControl.h>
int DIN = 13;
int CS =  12;
int CLK = 11;

#define VRx A1
#define VRy A0

byte up[] = {B00010000, B00111000, B01010100, B10010010, B00010000, B00010000, B00010000, B00010000};
byte down[] = {B00001000, B00001000, B00001000, B00001000, B01001001, B00101010, B00011100, B00001000};
byte right[] = {B00001000, B00000100, B00000010, B11111111, B00000010, B00000100, B00001000, B00000000};
byte left[] = {B00000000, B00010000, B00100000, B01000000, B11111111, B01000000, B00100000, B00010000};
byte blank[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

LedControl lc = LedControl(DIN, CLK, CS, 1);

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  Serial.begin(9600);
}

void loop() {
  int x_data = analogRead(VRx);
  int y_data = analogRead(VRy);
  // Serial.println(x_data);
  // delay(200);
  // Serial.println(y_data);
  // delay(200);
  if (x_data >= 600){
    printByte(left);
  }else if (x_data <= 400){
    printByte(right);
  }else if (y_data >= 600){
    printByte(up);
  }else if (y_data <= 400){
    printByte(down);
  }else{
    printByte(blank);
  }

  }

void printByte(byte character [])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}
