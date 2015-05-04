#ifndef __V2_LIBS_H_
#define __V2_LIBS_H_

#include <I2cInput.h>
#include <UnixCmd.h>

#include <AnalogInput.h>
#include <Arduino.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <RtcInput.h>
#include <SdData.h>

// Se definen valores para los sensores de corriente y voltaje
#define MAX_BATT_V 12.6
#define MAX_VOLTS 3.31
#define R1_R2 12940.0
#define R2 3210.0

#define BUFFER_MAX   100
#define MAX_ROWS     10

class V2Libs {
private:
    // Communications
    SerialEndpoint ble;
    SerialEndpoint pcComm;

    // Sensors
    Mcp9808Sensor tempSensor;
    AnalogInput currentSensor;
    AnalogInput voltageSensor;
    GpsInput gpsInput;

    // Command recognition
    UnixCmd unixCmd;

    // File writing
    SdWriter sdWriter;
    int rowsCount;

    // Real time clock
    RtcClock rtcClock;

public:
    V2Libs();
    ~V2Libs();

    void sendTemperature();
    void sendCurrent();
    void sendVoltage();
    float toAmps(float vout3v);
    float toVolts(int reading);
    void sendGpsStruct();
    void sendLatitude();
    void sendLongitude();
    void sendAltitude();

    void setup();
    void loop();
};

#endif // __V2_LIBS_H_
