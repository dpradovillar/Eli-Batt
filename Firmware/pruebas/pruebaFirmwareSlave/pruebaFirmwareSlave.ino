#define TARGET_BOARD    BOARD_UNO
#define TARGET_FIRMWARE FIRMWARE_SLAVE
#define TARGET_DEBUG    true

#include <elibatt_config.h>

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

#if TARGET_DEBUG
SerialEndpoint pcComm;
#endif

void setup() {
#if TARGET_DEBUG
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
  pcComm.print("Connected to:");
  pcComm.println(PROFILE_LABEL);
#endif
  
  firmwareSlave.setup(
    COMM1_RX,COMM1_TX,COMM1_SPEED,
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    CURRENT_PIN, VOLTAGE_PIN
#if TARGET_DEBUG
    ,&pcComm
#endif
  );
}

void loop() {
  firmwareSlave.loop();
}

