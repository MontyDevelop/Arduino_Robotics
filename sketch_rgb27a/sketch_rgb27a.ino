int R = 9, G = 10, B = 11;

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
}

void loop() {
  for(int r=0; r<255; r++) { analogWrite(R,r); analogWrite(G,255-r); delay(5); }
  for(int b=0; b<255; b++) { analogWrite(G,b); analogWrite(B,255-b); delay(5); }
  for(int g=0; g<255; g++) { analogWrite(B,g); analogWrite(R,255-g); delay(5); }
}
