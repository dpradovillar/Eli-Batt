#include <Utils.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  uint16_t crc = 12345;
  Serial.print("crc:");
  Serial.println(crc);
  
  byte buff[2];
  Utils::toByte(crc, buff);
  
  Serial.print("code0:"); Serial.println((int)buff[0]);
  Serial.print("code1:"); Serial.println((int)buff[1]);
  
  char buffer[5];
  buffer[4] = 0;
  Utils::toHex(buffer, buff[0]);
  Utils::toHex(buffer+2, buff[1]);
  
  Serial.print("Hex:");
  Serial.println(buffer);
  
}

void loop() {}
