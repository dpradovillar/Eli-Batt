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

#define TYPE_GET_DATA  8
#define TYPE_SCAN     16
#define TYPE_SET_ID   32
#define TYPE_GET_ID   64

/**
 * Sent from master, to all slaves, to discover theirs ids. The slave receives it, emits a response, and passes it on.
 *
 * Message(crc,SCAN_MESSAGE,<master id>,<empty>,<empty>)
 */
#define SCAN_MESSAGE        1

// TODO: remove this message as it's unnecessary
/**
 * Sent from master, automatically to itself, to be registered in the bank data object that synchronizes slave data.
 *
 * Request : Message(crc,MASTER_ID_REGISTER,<master id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_ID_REGISTER,<master id>,<master id>,<ok|error>)
 */
#define MASTER_ID_REGISTER  2

/**
 * Sent from master, to an specific slave, to change its id. The master must know the previous slave id.
 *
 * Request : Message(crc,SLAVE_ID_WRITE,<master id>,<old slave id>,<new slave id>)
 * Response: Message(crc,SLAVE_ID_WRITE,<old slave id>,<master id>,<ok|error>)
 */
#define SLAVE_ID_WRITE      3

/**
 * Sent from master|slave|pc to know the id of the first board in the chain. Every board should process this message
 * immediately and should report its id as a response.
 *
 * Request : Message(crc,SCAN_ID_READ,<master|slave|pc id>,<empty>,<empty>)
 * Response: Message(crc,SCAN_ID_READ,<recognized id>,<master|slave|pc>,<empty>)
 */
#define SCAN_ID_READ       4

/**
 * Sent from master, to an specific slave, to check if the slave is responding with its correct id.
 *
 * Request : Message(crc,SLAVE_ID_CHECK,<master id>,<slave id>,<empty>)
 * Response: Message(crc,SLAVE_ID_CHECK,<slave id>,<master id>,<empty>)
 */
#define SLAVE_ID_CHECK      5

/**
 * Sent from master, to an specific slave, to read the current sensor values (temperature, current, voltage).
 *
 * Request : Message(crc,SLAVE_DATA_READ,<master id>,<slave id>,<empty>)
 * Response: Message(crc,SLAVE_DATA_READ,<slave id>,<master id>,<temperature|current|voltage>)
 */
#define SLAVE_DATA_READ     6

/**
 * Sent from pc to master, to read the current sensor values (temperature, current, voltage).
 *
 * Request : Message(crc,MASTER_DATA_READ,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_DATA_READ,<master id>,<pc id>,<empty>)
 */
#define MASTER_DATA_READ    7

/**
 * Sent from
 *
 */
#define MASTER_DATA_GATHER  8

/**
 *
 *
 */
#define MASTER_GPS_GET      9

/**
 * Message sent from pc to master, to set the current RTC time being served by the board.
 *
 */
#define MASTER_RTC_SET      10

/**
 * Message sent from pc to master, to read the current RTC time being served by the board.
 *
 */
#define MASTER_RTC_GET      11

/**
 *
 *
 */
#define MASTER_ID_WRITE     12

/**
 *
 *
 */
#define MASTER_ID_READ      13

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
class Message /* : public DataObject*/ {
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
