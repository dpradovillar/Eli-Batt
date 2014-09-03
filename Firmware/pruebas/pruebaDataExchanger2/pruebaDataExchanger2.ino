#include <SoftwareSerial.h>

SoftwareSerial ss(6, 7);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  ss.begin(9600);
}

void loop() {
  bool entered = true;
  while(ss.available()){
    entered = true;
    byte c = ss.read();
    if (c < 32){
      Serial.print("code ");
      Serial.print((int)c);
      Serial.println(" ");
    } else {
      Serial.print(c);
      Serial.print("(");
      Serial.print((uint16_t)c);
      Serial.println(") ");
    }
    if(entered){
      Serial.println();
    }
  }
}
