#define TARGET_BOARD    BOARD_UNO
#define TARGET_FIRMWARE FIRMWARE_SLAVE
#define TARGET_DEBUG    true

#include <elibatt_config.h>

#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <BankData.h>
#include <DataExchanger.h>
#include <DataStream.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <FirmwareSlave.h>
#include <MemoryFree.h>
#include <I2cInput.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

FirmwareSlave firmwareSlave;
SerialEndpoint pcComm;

void setup() {
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
  pcComm.print("Connected to:");
  pcComm.println(PROFILE_LABEL);

  pcComm.print("freeMemory():");
  pcComm.println(freeMemory());
  
  
  
  firmwareSlave.setup(
    COMM1_RX,COMM1_TX,COMM1_SPEED,
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    CURRENT_PIN, VOLTAGE_PIN,
    DEBUG_PIN, &pcComm
  );
  //while(1);
}

void loop() {
  firmwareSlave.loop();
  
}

