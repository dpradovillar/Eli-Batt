#ifndef __FIRMWARE_MASTER_H_
#define __FIRMWARE_MASTER_H_

#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <BankData.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <Firmware.h>
#include <GpsInput.h>
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

class FirmwareMaster : public Firmware {
protected:
    /** SD Writer. */
    BankData m_bank_data;

    RtcClock m_rtc_clock;

    GpsInput m_gps;

    SerialEndpoint m_ble;

    void propagateForward(Message &msg);
    void propagateBackward(Message &msg);

    void set(Message &msg, uint32_t from, uint32_t to, byte type);
    bool printDbg(const char *s);

public:
    FirmwareMaster();

    void setup(int dbg_rx, int dbg_tx, int dbg_bauds, int rx2, int tx2, int bauds2,
            int ble_rx, int ble_tx, int bauds_ble,
            int currentSensorPin, int voltageSensorPin, int sdCsPin, uint32_t fileDuration,
            int debugPin, SerialEndpoint *dbgEndpoint=NULL);

    void loop();

    virtual bool handleMessage(Message &message);

    bool processGpsCommand(Message &message);

    void process(char cmd);
};

#endif // __FIRMWARE_MASTER_H_
