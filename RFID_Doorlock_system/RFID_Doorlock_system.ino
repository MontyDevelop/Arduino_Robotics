#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo myservo;
int pos = 0;

#define SS_PIN 9
#define RST_PIN 8
#define LED_G 47 //define green LED pin
#define LED_R 49
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

 
void setup() 
{
  myservo.attach(22); // Servo motor pin
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.begin(16, 2);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "CA 1C D2 A6" || content.substring(1) == "93 6C 7E A5") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    lcd.print("Welcome  - Monty ");
    Serial.println();
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, LOW);
    myservo.write(0);
    delay(2000);
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    myservo.write(90);
    lcd.clear();
}
 
 else   {
    Serial.println(" Access denied");
    lcd.print(" Access denied");
    myservo.write(0);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, LOW);
    lcd.clear();
    
  }
} 
