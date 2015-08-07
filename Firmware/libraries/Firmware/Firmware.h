#ifndef __FIRMWARE_H_
#define __FIRMWARE_H_

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
#include <I2cInput.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include <Wire.h>

class Firmware : public Handler {
protected:
    EepromWriter m_eeprom_writer;
    uint32_t m_id;

    /** Previous device in the series. */
    SerialEndpoint m_comm_a;
    DataStreamReader m_dsr_a;
    DataStreamWriter m_dsw_a;

    /** Next device in the series. */
    SerialEndpoint m_comm_b;
    DataStreamReader m_dsr_b;
    DataStreamWriter m_dsw_b;

    DualSerialEndpoint m_comm_a_wrapper;
    DualSerialEndpoint m_comm_b_wrapper;

    /** For processing and forward/reply messages. */
    DataExchanger m_dex;

    /** Sensors. */
    Mcp9808Sensor m_temp_sensor; /** UNO uses sda=A4, scl=A5. */
    AnalogInput m_current_sensor;
    AnalogInput m_voltage_sensor;

    /** Debugging endpoint. */
    Debugger d;

    //void propagateMessage();
	void packSensorValues(byte *buffer6Bytes);

public:
    Firmware();
    virtual ~Firmware();

    void setup(
        int rx1, int tx1, int bauds1,
        int rx2, int tx2, int bauds2,
        int currentSensorPin, int voltageSensorPin,
        int debugPin, SerialEndpoint *additionalEndpoint, SerialEndpoint *dbgEndpoint
    );

    void loop();

    virtual bool handleMessage(Message &message) = 0;
};

#endif // __FIRMWARE_H_
