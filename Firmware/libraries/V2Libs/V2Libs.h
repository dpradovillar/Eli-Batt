#ifndef __V2_LIBS_H_
#define __V2_LIBS_H_

#include <I2cInput.h>
#include <Cmd.h>

#include <AnalogInput.h>
#include <Arduino.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <RtcInput.h>
#include <SdData.h>
#include <MemoryFree.h>

// Se definen valores para los sensores de corriente y voltaje
#define MAX_BATT_V 12.6
#define MAX_VOLTS 3.31
#define R1_R2 12940.0
#define R2 3210.0

#define BUFFER_MAX   100
#define MAX_ROWS     900 // 15min 

// Serial
#define PC_COMM Serial

// Serial2
#define GPS_COMM Serial2

// Serial3
#define BLE_COMM Serial

class V2Libs {
private:
    // Sensors
    Mcp9808Sensor tempSensor;
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

    float toAmps(float vout3v);
    float toVolts(int reading);

    int sendThrough(GpsStruct &gdata, HardwareSerial *se);
    int sendAnalogValues(HardwareSerial *se);

public:
    V2Libs();
    ~V2Libs();

    void maybeDebug(const char *s);

    void setupPcComm();         bool pcCommEnabled;
    void setupBleComm();        bool bleCommEnabled;
    void setupTempSensor();     bool tempSensorEnabled;
    void setupCurrentSensor();  bool currentSensorEnabled;
    void setupVoltageSensor();  bool voltageSensorEnabled;
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
    DateTime getDateTime();

    void setup();
    void loop();
};

#endif // __V2_LIBS_H_
