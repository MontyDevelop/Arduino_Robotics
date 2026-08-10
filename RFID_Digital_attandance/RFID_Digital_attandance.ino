#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

#define GREEN_LED 6
#define RED_LED 7

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,16,2);

// Change according to your card UID
byte validUID[4] = {0x33,0xCA,0x24,0x15};

void setup()
{
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Attendance");
  lcd.setCursor(0,1);
  lcd.print("Scan Card...");
}

void loop()
{

  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);

  if(!rfid.PICC_IsNewCardPresent())
    return;

  if(!rfid.PICC_ReadCardSerial())
    return;

  bool match=true;

  for(byte i=0;i<4;i++)
  {
    if(rfid.uid.uidByte[i]!=validUID[i])
    {
      match=false;
      break;
    }
  }

  lcd.clear();

  if(match)
  {
    digitalWrite(GREEN_LED,HIGH);

    lcd.setCursor(0,0);
    lcd.print("Attendance");

    lcd.setCursor(0,1);
    lcd.print("Rahul Present");
  }
  else
  {
    digitalWrite(RED_LED,HIGH);

    lcd.setCursor(0,0);
    lcd.print("ALERT!");

    lcd.setCursor(0,1);
    lcd.print("Invalid Card");
  }

  delay(3000);

  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Attendance");
  lcd.setCursor(0,1);
  lcd.print("Scan Card...");

  rfid.PICC_HaltA();
}