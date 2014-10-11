#define TARGET_BOARD    BOARD_UNO
#define TARGET_FIRMWARE FIRMWARE_SLAVE
#define TARGET_DEBUG    0

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
#include <RtcInput.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

FirmwareSlave firmwareSlave;

#if TARGET_DEBUG
SerialEndpoint pcComm;
#endif

void setup() {
#if TARGET_DEBUG
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
  pcComm.print(F("Connected to:"));
  pcComm.println(PROFILE_LABEL);

  pcComm.print(F("freeMemory():"));
  pcComm.println(freeMemory());
#endif
  
  firmwareSlave.setup(
    COMM1_RX,COMM1_TX,COMM1_SPEED,
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    CURRENT_PIN, VOLTAGE_PIN,
    DEBUG_PIN, 
#if TARGET_DEBUG
    &pcComm
#else
    NULL
#endif
  );
}

void loop() {
  firmwareSlave.loop();
}

