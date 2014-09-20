#define TARGET_BOARD    BOARD_MEGA
#define TARGET_FIRMWARE FIRMWARE_MASTER
#define TARGET_DEBUG    true

#include <elibatt_config.h>

#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <BankData.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <FirmwareMaster.h>
#include <MemoryFree.h>
#include <I2cInput.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

FirmwareMaster firmwareMaster;

#if TARGET_DEBUG
SerialEndpoint pcComm;
#endif

void setup() {
#if TARGET_DEBUG
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
//              pcComm.print(freeMemory()); pcComm.println("FM");
  pcComm.print("Connected to:");
  pcComm.println(PROFILE_LABEL);
  pcComm.flush();
//              pcComm.print(freeMemory()); pcComm.println("FM");
#endif

  firmwareMaster.setup(
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    CURRENT_PIN, VOLTAGE_PIN,
    SD_CS_PIN, FILE_DURATION
#if TARGET_DEBUG
    ,&pcComm
#endif
  );
  
//  pcComm.print(freeMemory()); pcComm.println("FM");
  
//  while(1);
}

void loop() {
#if TARGET_DEBUG
  // TODO(rtapiapincheira): convert this serial comm into a SampleCollector (periodic task).
  if (pcComm.available()) {
    firmwareMaster.process(pcComm.read(), &pcComm);
  }
#endif
  firmwareMaster.loop();
  
//  delay(1000);
  
//  pcComm.print(freeMemory()); pcComm.println("FM");
}
