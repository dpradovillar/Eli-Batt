#include <EEPROM.h>
#include <Utils.h>
#include <SimpleCrc.h>
#include <EepromId.h>

EepromId eepromId;

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

uint32_t id;
void loop() {
  if(Serial.available()) {
    char c = Serial.read();
    if (c == 'w') {
      Serial.println("Writing id to eeprom.");
      while(Serial.available() < 4);
      byte buff[4];
      buff[0] = Serial.read();
      buff[1] = Serial.read();
      buff[2] = Serial.read();
      buff[3] = Serial.read();
      
      Serial.print("Read id from pc: (");
      for(int i=0; i<4; i++) Serial.print((char)buff[i]);
      Serial.println(")");
      id = Utils::toInt32(buff);
      Serial.println(id, DEC);
      /*
      Serial.println("Bytes read back from the just converted uint32_t:");
      Utils::toByte(id, buff);
      Serial.print("(");
      for(int i=0; i<4; i++) Serial.print((char)buff[i]);
      Serial.println(")");
      id = Utils::toInt32(buff);
      Serial.print("Converted again: ");
      Serial.println(id, DEC);
      */
      
      eepromId.writeId(id);
    } else if (c == 'r') {
      Serial.println("Reading id from eeprom.");
      //id = eepromId.readId();
      Serial.print("Read back: ");
      Serial.println(id, DEC);
    }
  }
}
