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

class FirmwareMaster : public Handler {
private:
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

    /** Sensors. */
    Mcp9808Sensor m_temp_sensor; /** Uses sda=A4, scl=A5 on UNO. */
    AnalogInput m_current_sensor;
    AnalogInput m_voltage_sensor;

    /** SD Writer. */
    BankData m_bank_data;

    /** Debugging endpoint. */
    Debugger d;

    Message m_the_message;
SerialOutputHandler m_soh;

    // RTC? GPS? BLE?

public:
    FirmwareMaster();

    void setup(int rx2, int tx2, int bauds2, int currentSensorPin, int voltageSensorPin,
            int sdCsPin, uint32_t fileDuration, SerialEndpoint *dbgEndpoint=NULL);

    void loop();

    bool handleMessage(Message *message);

    void process(int next, SerialEndpoint *endpoint);
    void requestWriteId(byte *targetId, byte *newId);
    void requestReadId(byte *targetId);
    void requestData(byte *targetId);

private:
    void propagateMessage();
};

#endif // __FIRMWARE_MASTER_H_
