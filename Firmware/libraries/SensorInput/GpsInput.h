#ifndef __GPS_INPUT_H_
#define __GPS_INPUT_H_

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <Debugger.h>
#include <elibatt_config.h>
#include <Endpoint.h>
#include <ArduinoSoftwareSerial.h>
#include <Utils.h>

/**
 * Structure for passing GPS data around.
 */
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

    uint32_t int_lat;      uint32_t dec_lat;
    uint32_t int_lon;      uint32_t dec_lon;
    uint32_t int_speed;    uint32_t dec_speed;
    uint32_t int_angle;    uint32_t dec_angle;
    uint32_t int_altitude; uint32_t dec_altitude;

    byte satellites;

};

/**
 * Wrapper for the GPS Adafruit library.
 */
class GpsInput {
private:
    byte m_ok;

    Adafruit_GPS m_gps;

    // Last data pulled from the gps device.
    GpsStruct m_data;

public:
    GpsInput();

    bool setup(HardwareSerial *serial, int gps_bauds);

    /**
     * Tries to read and buffers internally new gps data.
     * <p/>
     * Make sure to call this method regularly in the main loop (not inside a callback nor every
     * 1 second or so), it should be called as fast as possible.
     */
    void loop();

    /**
     * Returns true if there's data available. Otherwise, it returns false.
     */
    bool available();

    /**
     * Returns the last data structure pulled by this class from the device.
     */
    GpsStruct getData();
};

#endif // __GPS_INPUT_H_
