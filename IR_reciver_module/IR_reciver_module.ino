#include <IRremote.h>

const int RECV_PIN = 2;
int led = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define ENA 5
#define IN1 8
#define IN2 9


void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  irrecv.enableIRIn();
  irrecv.blink13(true);

}

void loop(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
        // if (results.value == 0xFF6897){
        //   digitalWrite(led, LOW);
        // }else if (results.value == 0xFF30CF){
        //   digitalWrite(led, HIGH);
        // }
  }
}