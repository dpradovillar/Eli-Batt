#include <Utils.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  test1();  
}

void test1() {

  
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
  
  uint32_t x = 315442569;
  
  char buffer2[9];
  Utils::toHex(buffer2, x);
  buffer2[8] = 0;
  Serial.print("Hex2:");
  Serial.println(buffer2);
}

void printPadded(uint32_t val, int len, char toPad) {
  char buff[32];
  Utils::leftPad(val, buff, len, toPad);
  buff[len] = 0;
  Serial.println(buff);
}

void test2() {
  printPadded(1, 2, '.');
  printPadded(1, 3, '.');
  printPadded(1, 4, '.');
  
  printPadded(123, 4, '.');
  printPadded(123, 8, '.');
  
  printPadded(1234, 4, '.');
  printPadded(12345, 4, '.');
}

void loop() {}
