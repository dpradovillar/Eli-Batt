#ifndef __V2_LIBS_SLAVE_H_
#define __V2_LIBS_SLAVE_H_

#include <I2cInput.h>
#include <Cmd.h>

#include <AnalogInput.h>
#include <Arduino.h>
#include <Endpoint.h>
#include <MemoryFree.h>
#include <V2LibsComm.h>
#include <EEPROM.h>
#include <Utils.h>
#include <SimpleCrc.h>
#include <EepromWriter.h>

#define MAX_BATT_V 12.6
#define MAX_VOLTS 3.31
#define R1_R2 12940.0
#define R2 3210.0



// Serial
#define PC_COMM Serial

// softwareSerial
#define SOFT_COMM softwareSerial

class V2LibsSlave {
private:
    SoftwareSerial SOFT_COMM;

    // Sensors
    Mcp9808Sensor tempSensor;
    AnalogInput currentSensor;
    AnalogInput voltageSensor;

    // Communication logic
    V2LibsComm v2libsComm;
    EepromWriter eepromWriter;
    uint32_t eepromId;

    // Command recognition
    Cmd cmd;

    float toAmps(float vout3v);
    float toVolts(int reading);

    int sendAnalogValues(HardwareSerial *se);

public:
    V2LibsSlave();
    ~V2LibsSlave();

    void maybeDebug(const char *s);

    void setupPcComm();         bool pcCommEnabled;
    void setupSoftComm();       bool softCommEnabled;
    void setupV2LibsComm();
    void setupId();
    void setupTempSensor();     bool tempSensorEnabled;
    void setupCurrentSensor();  bool currentSensorEnabled;
    void setupVoltageSensor();  bool voltageSensorEnabled;

    float getTemperature();
    float getCurrent();
    float getVoltage();

    void setup();
    void loop();
};

#endif // __V2_LIBS_SLAVE_H_
