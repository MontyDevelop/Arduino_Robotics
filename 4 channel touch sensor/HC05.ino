   /*
  Made on Jan 25, 2021
  By MehranMaleki @ Electropeak
  Home
*/

const int touchButton_1 = 4;
const int touchButton_2 = 5;
const int touchButton_3 = 6;
const int touchButton_4 = 7;


void setup() {
  Serial.begin(9600);

  pinMode(touchButton_1, INPUT);
  pinMode(touchButton_2, INPUT);
  pinMode(touchButton_3, INPUT);
  pinMode(touchButton_4, INPUT);
}

void loop() {
Serial.print(digitalRead(touchButton_1));
Serial.print("\t");
Serial.print(digitalRead(touchButton_2));
Serial.print("\t");
Serial.print(digitalRead(touchButton_3));
Serial.print("\t");
Serial.println(digitalRead(touchButton_4));

delay(1000);
}
