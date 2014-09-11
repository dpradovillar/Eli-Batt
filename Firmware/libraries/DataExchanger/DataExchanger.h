#ifndef __DATA_EXCHANGER_H_
#define __DATA_EXCHANGER_H_

#include <Endpoint.h>
#include <DataStream.h>
#include <Debugger.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include "Arduino.h"

#define ID_DATA_LENGTH 4
#define CUSTOM_MESSAGE_DATA_LENGTH 8

#define MESSAGE_SIZE 20 // 2+ID_DATA_LENGTH+ID_DATA_LENGTH+1+CUSTOM_MESSAGE_DATA_LENGTH+1

// TODO(rtapiapincheira): convert into true flags
#define STATUS_IN_TRANSIT    1
#define STATUS_ACKNOWLEDGED  2
#define STATUS_UNDELIVERABLE 4

#define TYPE_DATA  8
#define TYPE_SCAN  16

/**
 * Base class for all the different kind of messages sent between any pair of boards. It has a basic
 * issuer and address mechanism to specify who sends the message, and who is the receiver.
 * <p/>
 * In cases where the message can't be delivered (the message reached the end of the chain of
 * boards, without finding the target id), the message is marked as undeliverable and it is re send
 * to the emitter.
 * <p/>
 * In cases where the delivery is done successfully, the message is marked as acknowledged, and is
 * sent back to the emitter.
 */
class Message : public DataObject {
public:
    uint16_t m_crc;
    byte m_type;
    byte m_status;
    byte m_fromId[ID_DATA_LENGTH];
    byte m_targetId[ID_DATA_LENGTH];
    byte m_data[CUSTOM_MESSAGE_DATA_LENGTH];

    ~Message();
    size_t writeTo(DataStreamWriter *dsw);
    size_t readFrom(DataStreamReader *dsr);
    uint16_t calculateCrc();
    size_t writeAsciiTo(DataStreamWriter *dsw);
    void swapIds();
}; // 18bytes in total

class Handler {
public:
    virtual ~Handler();
    virtual bool handleMessage(Message *message) = 0;
};

class SerialOutputHandler : public Handler {
public:
    bool handleMessage(Message *message);
};

class DataExchanger {
private:
    /** Connection using hardware Serial pins.*/
    DataStreamReader *m_hardwareReader;
    DataStreamWriter *m_hardwareWriter;

    /** Connection using software Serial pins.*/
    DataStreamReader *m_softwareReader;
    DataStreamWriter *m_softwareWriter;

    byte m_id[ID_DATA_LENGTH];
    Handler *m_handler;

    /** Endpoint for debugging and error messages.*/
    Debugger d;

    void process(Message *message, DataStreamWriter *readFromLine, DataStreamWriter *opposingLine);
    void transmit(DataStreamWriter *dsw, Message *message);

public:
    DataExchanger();

    void setup(byte *id, Handler *handler);
    void setupHardware(DataStreamReader *dsr, DataStreamWriter *dsw);
    void setupSoftware(DataStreamReader *dsr, DataStreamWriter *dsw);

    void loop();
};

#endif // __DATA_EXCHANGER_H_
