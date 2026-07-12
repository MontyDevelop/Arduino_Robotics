#include <Arduino.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif 


const int pHSensorPin = A0;
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(SCL, SDA, U8X8_PIN_NONE);

void setup(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
}

void loop(void)
{
  int analogValue = analogRead(pHSensorPin);
  float slope = 0.0549;       
  int referenceAnalog = 827;  
  float pH = 7.00 - (analogValue - referenceAnalog) * slope;

  String ph_string_data = String(pH, 2);
  u8x8.setFont(u8x8_font_chroma48medium8_r);



  u8x8.drawString(1,1,"Himank Team");
  u8x8.drawString(1,3,"PH Value: ");
  u8x8.setCursor(10,3);
  u8x8.print(ph_string_data);
  u8x8.refreshDisplay();		
  delay(1000);
}
