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
#include <Firmware.h>
#include <FirmwareMaster.h>
#include <MemoryFree.h>
#include <I2cInput.h>
#include <SampleCollector.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

FirmwareMaster firmwareMaster;

SampleClock sampleClock;
SerialEndpoint pcComm;

class MasterCallback : public SampleCallback {
public:
  virtual ~MasterCallback() {}
  virtual void eventDetected(uint32_t usecs){
    //firmwareMaster.process('g');
  }
};

MasterCallback masterCallback;

void setup() {
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
  pcComm.print("Connected to:");
  pcComm.println(PROFILE_LABEL);
  pcComm.flush();
  pcComm.print("Available memory:");
  pcComm.println(freeMemory());
  pcComm.flush();
  sampleClock.setup(0, &masterCallback);
  sampleClock.setPeriod(MASTER_CLOCK_DELAY);

  firmwareMaster.setup(
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    CURRENT_PIN, VOLTAGE_PIN,
    SD_CS_PIN, FILE_DURATION,
    DEBUG_PIN ,&pcComm
  );
}

void loop() {
  if (pcComm.available()) {
     firmwareMaster.process((char)pcComm.read());
  }
  firmwareMaster.loop();
  sampleClock.loop();
}
