#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <DataExchanger.h>
#include <DataStream.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <FirmwareSlave.h>
#include <I2cInput.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

FirmwareSlave firmwareSlave;
SerialEndpoint pcComm;

int currentSensorPin = 3;
int voltageSensorPin = 4;

void setup() {
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();
  
  firmwareSlave.setup(8,9, 6,7, currentSensorPin, voltageSensorPin, 9600, &pcComm);
}

void loop() {
  firmwareSlave.loop();
}

