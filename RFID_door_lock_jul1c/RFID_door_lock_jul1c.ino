N#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6

MFRC522 rfid(SS_PIN, RST_PIN);
Servo doorServo;

// Replace with YOUR RFID Card UID
byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

void setup()
{
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  Serial.println("Scan Your RFID Card...");
}

void loop()
{
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print("Card UID: ");

  for (byte i = 0; i < rfid.uid.size; i++)
  {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  bool access = true;

  for (byte i = 0; i < 4; i++)
  {
    if (rfid.uid.uidByte[i] != authorizedUID[i])
    {
      access = false;
      break;
    }
  }

  if (access)
  {
    Serial.println("Access Granted");

    Serial.println("Door Unlocked");

    doorServo.write(90);

    delay(3000);

    doorServo.write(0);

    Serial.println("Door Locked");
  }
  else
  {
    Serial.println("Access Denied");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}