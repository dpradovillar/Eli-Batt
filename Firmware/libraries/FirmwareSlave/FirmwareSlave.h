#ifndef __FIRMWARE_SLAVE_H_
#define __FIRMWARE_SLAVE_H_

#include "Arduino.h"

#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <I2cInput.h>
#include <SimpleCrc.h>
#include <Utils.h>

class FirmwareSlave : public Handler {
private:
	/** For retrieving the ID of the master burned in the EEPROM memory. */
	EepromWriter m_eeprom_writer;
    byte m_id[4];

    /** Previous device in the series. */
    SerialEndpoint m_comm_a;
    DataStreamReader m_dsr_a;
    DataStreamWriter m_dsw_a;

    /** Next device in the series. */
    SerialEndpoint m_comm_b;
    DataStreamReader m_dsr_b;
    DataStreamWriter m_dsw_b;

    /** For processing and forward/reply messages. */
    DataExchanger m_dex;

    /** Sensors .*/
    Mcp9808Sensor m_temp_sensor; /** Uses sda=A4, scl=A5 on UNO. */
    AnalogInput m_current_sensor;
    AnalogInput m_voltage_sensor;

    /** For message verification. */
    SimpleCrc m_simple_crc;

    Debugger d;

public:
    FirmwareSlave();

    void setup(int rx1, int tx1, int rx2, int tx2, int currentSensorPin, int voltageSensorPin,
            int bauds, Endpoint *debuggerEndpoint);

    void loop();

    virtual bool handleMessage(Message *message);

    bool getData(Message *message);
    bool getId(Message *message);
    bool setId(Message *message);
};

#endif // __FIRMWARE_SLAVE_H_
