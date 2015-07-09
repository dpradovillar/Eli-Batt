#ifndef __V2_LIBS_H_
#define __V2_LIBS_H_

#include <I2cInput.h>
#include <Cmd.h>
#include <OneWireInput.h>
#include <AnalogInput.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <ParserComm.h>
#include <RtcInput.h>
#include <SdData.h>
#include <MemoryFree.h>

// Se definen valores para los sensores de corriente y voltaje
#define MAX_BATT_V 12.6
#define MAX_VOLTS 5.00 // ? V entre GND y VCC?
#define R1_R2 335.1
#define R2 118.3

#define BUFFER_MAX   100
#define MAX_ROWS     60 // 15min

#define RELAY_DIGITAL_PIN1 13
#define RELAY_DIGITAL_PIN2 36

#define RELEASE_BOARD 1

// Serial
#define PC_COMM Serial

// Serial2
#define GPS_COMM Serial2

// Serial3
#if RELEASE_BOARD
#   define BLE_COMM Serial3
#else
#   define BLE_COMM Serial
#endif

// SoftwareSerial
#define BANK_COMM m_bank

class V2Libs {
private:
    // Sensors
    Ds1820Sensor tempSensor;
    AnalogInput currentSensor;
    AnalogInput voltageSensor;
    GpsInput gpsInput;

    // Command recognition
    Cmd cmd;

    // File writing
    SdWriter sdWriter;
    int rowsCount;

    // Real time clock
    RtcClock rtcClock;

    // Bank communication
    SoftwareSerial BANK_COMM;
    ParserComm parserComm;

    EepromWriter eepromWriter;
    uint32_t eepromId;

    int lastDigitalWrite;

    float toAmps(float vout3v);
    float toVolts(int reading);

    int sendThrough(GpsStruct &gdata, HardwareSerial *se);
    int sendAnalogValues(HardwareSerial *se);

    void clearSlaveFlags();

public:
    V2Libs();
    ~V2Libs();

    void maybeDebug(const char *s);

    void setupPcComm();         bool pcCommEnabled;
    void setupBleComm();        bool bleCommEnabled;
    void setupBankComm();       bool bankCommEnabled;
    void setupParserComm();
    void setupEepromWriter();
    void setupTempSensor();     bool tempSensorEnabled;
    void setupCurrentSensor();
    void setupVoltageSensor();
    void setupGps();            bool gpsEnabled;
    void setupSdWriter();       bool sdWriterEnabled;
    void setupRtcClock();       bool rtcClockEnabled;

    float getTemperature();
    float getCurrent();
    float getVoltage();
    GpsStruct getGpsStruct();
    float getLatitude();
    float getLongitude();
    float getAltitude();
    MyDate getDateTime();

    // Slave-related methods
    float getAverageTemperature();
    float getAverageCurrent();
    float getAverageVoltage();
    int getSlaveCount();
    int sendTemperatureList(HardwareSerial *se);
    int sendCurrentList(HardwareSerial *se);
    int sendVoltageList(HardwareSerial *se);
    int sendIdList(HardwareSerial *se);

    void setup();
    void loop();
};

#endif // __V2_LIBS_H_
