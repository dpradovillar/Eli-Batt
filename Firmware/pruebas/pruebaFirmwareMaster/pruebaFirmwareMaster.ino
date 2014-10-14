#define TARGET_BOARD    BOARD_MEGA
#define TARGET_FIRMWARE FIRMWARE_MASTER
#define TARGET_DEBUG    0

#include <elibatt_config.h>

#include <Adafruit_GPS.h>
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
#include <RtcInput.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <SampleCollector.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

FirmwareMaster firmwareMaster;

SampleClock sampleClock;

#if TARGET_DEBUG
SerialEndpoint pcComm;
#endif

class MasterCallback : public SampleCallback {
public:
  virtual ~MasterCallback() {}
  virtual void eventDetected(uint32_t usecs){
      firmwareMaster.process('g');
  }
};

MasterCallback masterCallback;

void setup() {
#if TARGET_DEBUG
  pcComm.setup(DEBUG_RX, DEBUG_TX, DEBUG_SPEED);
  pcComm.waitForConnection();
  pcComm.print(F("Connected to:"));
  pcComm.println(PROFILE_LABEL);
  pcComm.flush();
  pcComm.print(F("Available memory:"));
  pcComm.println(freeMemory());
  pcComm.flush();
#endif
  sampleClock.setup(0, &masterCallback);
  sampleClock.setPeriod(MASTER_CLOCK_DELAY);

  firmwareMaster.setup(
#if TARGET_DEBUG
    -1,-1,0,
#else
    0,1,9600,
#endif
    COMM2_RX,COMM2_TX,COMM2_SPEED,
    BLE_RX, BLE_TX, BLE_BAUD,
    CURRENT_PIN, VOLTAGE_PIN,
    SD_CS_PIN, FILE_DURATION,
    DEBUG_PIN,
#if TARGET_DEBUG
    &pcComm
#else
    NULL
#endif
  );
}

void loop() {
#if TARGET_DEBUG
  if (pcComm.available()) {
     firmwareMaster.process((char)pcComm.read());
  }
#endif
  firmwareMaster.loop();
  sampleClock.loop();
}
