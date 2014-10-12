#ifndef __GPS_INPUT_H_
#define __GPS_INPUT_H_

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <Debugger.h>
#include <elibatt_config.h>
#include <Endpoint.h>
#include <ArduinoSoftwareSerial.h>
#include <Utils.h>

class GpsStruct {
public:
    uint16_t year;
    byte month;
    byte day;

    byte hour;
    byte minute;
    byte second;

    byte fix;
    byte quality;

    uint32_t int_lat; uint32_t dec_lat;
    uint32_t int_lon; uint32_t dec_lon;
    uint32_t int_speed; uint32_t dec_speed;
    uint32_t int_angle; uint32_t dec_angle;
    uint32_t int_altitude; uint32_t dec_altitude;

    byte satellites;

};

/**
 * Wrapper for the GPS Adafruit library.
 */
class GpsInput {
private:
    byte m_ok;
    Debugger d;

    Adafruit_GPS m_gps;
    GpsStruct m_data;

public:
    bool setup(HardwareSerial *serial, int gps_bauds, SerialEndpoint *debugEndpoint=NULL);

    void pull();
    bool available();
    GpsStruct getData();
};

#endif // __GPS_INPUT_H_
