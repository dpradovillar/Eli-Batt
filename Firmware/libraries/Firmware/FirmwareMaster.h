#ifndef __FIRMWARE_MASTER_H_
#define __FIRMWARE_MASTER_H_

#include "Arduino.h"

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
#include <I2cInput.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

class FirmwareMaster : public Firmware {
protected:
    /** SD Writer. */
    BankData m_bank_data;

    // RTC? GPS? BLE?

    void propagateMessage();

public:
    FirmwareMaster();

    void setup(int rx2, int tx2, int bauds2, int currentSensorPin, int voltageSensorPin,
            int sdCsPin, uint32_t fileDuration, int debugPin, SerialEndpoint *dbgEndpoint=NULL);

    void loop();

    virtual bool handleMessage(Message &message);

private:
};

#endif // __FIRMWARE_MASTER_H_
