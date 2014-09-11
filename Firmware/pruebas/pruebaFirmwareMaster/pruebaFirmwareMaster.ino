#include <EEPROM.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <FirmwareMaster.h>
#include <SimpleCrc.h>
#include <Utils.h>

FirmwareMaster firmwareMaster;

SerialEndpoint pcComm;

void setup() {
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();

  firmwareMaster.setup(8, 9, 9600, &pcComm); // omit the &pcComm parameter to disable debugging messages
}

void loop() {
  if (pcComm.available()) {
    firmwareMaster.process(pcComm.read());
  }
  firmwareMaster.loop();
}
