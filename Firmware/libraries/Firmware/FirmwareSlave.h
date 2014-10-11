#ifndef __FIRMWARE_SLAVE_H_
#define __FIRMWARE_SLAVE_H_

#include <elibatt_config.h>

#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <Firmware.h>
#include <I2cInput.h>
#include <SimpleCrc.h>
#include <Utils.h>

class FirmwareSlave : public Firmware {
protected:
    // All boards communication related stuff are included already in Firmware as protected fields.
public:
    FirmwareSlave();

    void setup(int rx1, int tx1, int bauds1, int rx2, int tx2, int bauds2, int currentSensorPin,
            int voltageSensorPin, int debugPin, SerialEndpoint *debuggerEndpoint=NULL);

    void loop();

    virtual bool handleMessage(Message &message);
    virtual bool forceDebugRetransmit();
};

#endif // __FIRMWARE_SLAVE_H_
