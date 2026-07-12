#include <IRremote.h>

const int RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);
decode_results results;

#define ENA 5
#define IN1 8
#define IN2 9

unsigned long code;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  irrecv.enableIRIn();

  stopMotor();
}

void loop() {

  if (irrecv.decode(&results)) {

    code = results.value;

    if (code != 0) {
      Serial.println(code, HEX);

      if (code == 0xFF906F) {
        forward();
      }

      else if (code == 0xFFE01F) {
        reverse();
      }

      else if (code == 0xFFA25D) {
        stopMotor();
      }
    }

    irrecv.resume();
  }
}

void forward() {
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void reverse() {
  analogWrite(ENA, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}