#include <EEPROM.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <FirmwareSlave.h>
#include <SimpleCrc.h>
#include <Utils.h>

FirmwareSlave firmwareSlave;

void setup() {
  firmwareSlave.setup(0, 1, 6, 7, 9600);
}

void loop() {
  firmwareSlave.loop();
}
