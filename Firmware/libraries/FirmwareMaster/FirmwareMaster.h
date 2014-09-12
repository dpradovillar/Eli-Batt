#ifndef __FIRMWARE_MASTER_H_
#define __FIRMWARE_MASTER_H_

#include "Arduino.h"

#include <EEPROM.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <Debugger.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <SimpleCrc.h>
#include <Utils.h>

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

    Message m_the_message;
SerialOutputHandler m_soh;
    Debugger d;

public:
    FirmwareMaster();

    void setup(int rx2, int tx2, int bauds, Endpoint *dbgEndpoint=NULL);

    void loop();

    bool handleMessage(Message *message);

    void process(int next, Endpoint *endpoint);
    void requestWriteId(byte *targetId, byte *newId);
    void requestReadId(byte *targetId);
    void requestData(byte *targetId);
};

#endif // __FIRMWARE_MASTER_H_
