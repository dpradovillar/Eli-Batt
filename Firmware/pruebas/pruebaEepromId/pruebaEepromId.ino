#include <EEPROM.h>
#include <Utils.h>
#include <SimpleCrc.h>
#include <EepromWriter.h>

/**
* A simple program to write a 4-byte id into the arduino eeprom, and to read it back.
*
* Load it in Arduino Uno, open  Serial console @ 9600 bauds and input:
* w1111 -> (will convert '1' '1' '1' '1' into a 32bit integer and write it to eeprom)
* r     -> (will read back the value)
*
* This sketch does not use additional pins, it's only the arduino uno connected through
* USB to a PC.
*
* A useful test is to send the command (for example, w1234), unplug the Arduino Uno,
* plug it back, and enter as first command the 'r'. It should show the id written in
* the eeprom.
*/

EepromWriter eepromWriter;

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
      
      eepromWriter.writeId(id);
    } else if (c == 'r') {
      Serial.println("Reading id from eeprom.");
      id = eepromWriter.readId();
      Serial.print("Read back: ");
      Serial.println(id, DEC);
    }
  }
}
