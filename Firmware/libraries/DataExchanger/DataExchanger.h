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

/**
 * Sent from master, to all slaves, to discover theirs ids. The slave receives it, emits a response,
 * and passes it on.
 *
 * Request : Message(crc,SCAN_MESSAGE,<master id>,<empty>,<empty>)
 * Response: Message(crc,SCAN_MESSAGE,<discovered id>,<master id>,<empty>)
 */
#define SCAN_MESSAGE        1

/**
 * Sent from master, to an specific slave, to change its id. The master must know the previous slave
 * id.
 *
 * Request : Message(crc,SLAVE_ID_WRITE,<master id>,<old slave id>,<new slave id>)
 * Response: Message(crc,SLAVE_ID_WRITE,<old slave id>,<master id>,<ok|error>)
 */
#define SLAVE_ID_WRITE      2

/**
 * Sent from master|slave|pc to know the id of the first board in the chain. Every board should
 * process this message
 * immediately and should report its id as a response.
 *
 * Request : Message(crc,SCAN_ID_READ,<master|slave|pc id>,<empty>,<empty>)
 * Response: Message(crc,SCAN_ID_READ,<recognized id>,<master|slave|pc id>,<empty>)
 */
#define SCAN_ID_READ       3

/**
 * Sent from master|pc, to an specific slave|<master|slave> respectively, to check if that board is
 * responding with its correct id.
 *
 * Request : Message(crc,SCAN_ID_CHECK,<master|pc id>,<slave|master id>,<empty>)
 * Response: Message(crc,SCAN_ID_CHECK,<slave|master id>,<master|pc id>,<slave|master id>)
 */
#define SCAN_ID_CHECK      4

/**
 * Sent from master, to an specific slave, to read the current sensor values (temperature, current,
 * voltage).
 *
 * Request : Message(crc,SLAVE_DATA_READ,<master id>,<slave id>,<empty>)
 * Response: Message(crc,SLAVE_DATA_READ,<slave id>,<master id>,<temperature|current|voltage>)
 */
#define SLAVE_DATA_READ     5

/**
 * Sent from pc to master, to read the current sensor values (temperature, current, voltage).
 *
 * Request : Message(crc,MASTER_DATA_READ,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_DATA_READ,<master id>,<pc id>,<temperature|current|voltage>)
 */
#define MASTER_DATA_READ    6

/**
 * Sent from pc to master, to read the current sensor values (temperature, current, voltage) of the
 * master and the slaves attached to it.
 *
 * Request : Message(crc,MASTER_DATA_GATHER,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_DATA_GATHER,<master id>,<pc id>,<ok|error>)
 */
#define MASTER_DATA_GATHER  7

/**
 * Sent from pc to master, to read the current GPS status.
 *
 * Request : Message(crc,MASTER_GPS_GET,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_GPS_GET,<master id>,<pc id>,<lat|lon>) TODO: include more info here
 */
#define MASTER_GPS_GET      8

/**
 * Sent from pc to master, to set the current RTC time being served by the board.
 *
 * Request : Message(crc,MASTER_RTC_SET,<pc id>,<master id>,<new datetime>)
 * Response: Message(crc,MASTER_RTC_SET,<master id>,<pc id>,<ok|error>)
 */
#define MASTER_RTC_SET      9

/**
 * Sent from pc to master, to read the current RTC time being served by the board.
 *
 * Request : Message(crc,MASTER_RTC_GET,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_RTC_GET,<master id>,<pc id>,<date time>)
 */
#define MASTER_RTC_GET      10

/**
 * Sent from pc to master, to set the id of the master device.
 *
 * Request : Message(crc,MASTER_ID_WRITE,<pc id>,<old master id>,<new master id>)
 * Response: Message(crc,MASTER_ID_WRITE,<old master id>,<pc id>,<ok|error>)
 */
#define MASTER_ID_WRITE     11

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
    void swapIds();
}; // MESSAGE_SIZE bytes in total

class Handler {
public:
    virtual ~Handler();

    /**
     * Should return true, if the message should be sent back. It the message is consumed, and no
     * response is necessary by part of DataExchanger, return false.
     */
    virtual bool handleMessage(Message &message) = 0;
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

    void process(Message &message, DataStreamWriter *readFromLine, DataStreamWriter *opposingLine);
    void transmit(DataStreamWriter *dsw, Message &message);

public:
    DataExchanger();

    void setup(byte *id, Handler *handler, SerialEndpoint *debugEndpoint);
    void setupHardware(DataStreamReader *dsr, DataStreamWriter *dsw);
    void setupSoftware(DataStreamReader *dsr, DataStreamWriter *dsw);

    void loop();
};

#endif // __DATA_EXCHANGER_H_
