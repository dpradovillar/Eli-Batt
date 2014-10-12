#ifndef __DATA_EXCHANGER_H_
#define __DATA_EXCHANGER_H_

#include <Endpoint.h>
#include <DataStream.h>
#include <Debugger.h>
#include <SimpleCrc.h>
#include <Utils.h>
#include "Arduino.h"

#define CUSTOM_MESSAGE_DATA_LENGTH 8

#define MESSAGE_SIZE 19 // 7+ID_DATA_LENGTH+CUSTOM_MESSAGE_DATA_LENGTH

/**
 * Sent from master, to all slaves, to discover theirs ids. The slave receives it, emits a response,
 * and passes it on.
 *
 * Request : Message(crc,SCAN_MESSAGE,<master id>,<empty>,<empty>)
 * Response: Message(crc,SCAN_MESSAGE_RESPONSE,<discovered id>,<master id>,<empty>)
 */
#define SCAN_MESSAGE            1
#define SCAN_MESSAGE_RESPONSE 101

/**
 * Sent from pc to master, to force it emit a SCAN_MESSAGE request with the master as target.
 *
 * Request : Message(crc,SCAN_MESSAGE_START,<pc id>,<master id>,<empty>)
 * Response: Message(crc,SCAN_MESSAGE_START_RESPONSE,<master id>,<pc id>,<empty>)
 */
#define SCAN_MESSAGE_START              2
#define SCAN_MESSAGE_START_RESPONSE   102

/**
 * Sent from master, to an specific slave, to change its id. The master must know the previous slave
 * id.
 *
 * Request : Message(crc,SLAVE_ID_WRITE,<master id>,<old slave id>,<new slave id>)
 * Response: Message(crc,SLAVE_ID_WRITE_RESPONSE,<old slave id>,<master id>,<ok|error>)
 */
#define SLAVE_ID_WRITE            3
#define SLAVE_ID_WRITE_RESPONSE 103

/**
 * Sent from master|slave|pc to know the id of the first board in the chain. Every board should
 * process this message
 * immediately and should report its id as a response.
 *
 * Request : Message(crc,SCAN_ID_READ,<master|slave|pc id>,<empty>,<empty>)
 * Response: Message(crc,SCAN_ID_READ_RESPONSE,<recognized id>,<master|slave|pc id>,<empty>)
 */
#define SCAN_ID_READ            4
#define SCAN_ID_READ_RESPONSE 104

/**
 * Sent from master|pc, to an specific slave|<master|slave> respectively, to check if that board is
 * responding with its correct id.
 *
 * Request : Message(crc,SCAN_ID_CHECK,<master|pc id>,<slave|master id>,<empty>)
 * Response: Message(crc,SCAN_ID_CHECK_RESPONSE,<slave|master id>,<master|pc id>,<slave|master id>)
 */
#define SCAN_ID_CHECK            5
#define SCAN_ID_CHECK_RESPONSE 105

/**
 * Sent from master, to an specific slave, to read the current sensor values (temperature, current,
 * voltage).
 *
 * Request : Message(crc,SLAVE_DATA_READ,<master id>,<slave id>,<empty>)
 * Response: Message(crc,SLAVE_DATA_READ_RESPONSE,<slave id>,<master id>,<temperature|current|voltage>)
 */
#define SLAVE_DATA_READ            6
#define SLAVE_DATA_READ_RESPONSE 106

/**
 * Sent from pc to master, to read the current sensor values (temperature, current, voltage).
 *
 * Request : Message(crc,MASTER_DATA_READ,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_DATA_READ_RESPONSE,<master id>,<pc id>,<temperature|current|voltage>)
 */
#define MASTER_DATA_READ            7
#define MASTER_DATA_READ_RESPONSE 107

/**
 * Sent from pc to master, to read the current sensor values (temperature, current, voltage) of the
 * master and the slaves attached to it.
 *
 * Request : Message(crc,MASTER_DATA_GATHER,<pc id>,<master id>,<empty>)
 * Response: Message(crc,MASTER_DATA_GATHER_RESPONSE,<master id>,<pc id>,<ok|error>)
 */
#define MASTER_DATA_GATHER            8
#define MASTER_DATA_GATHER_RESPONSE 108
#define MASTER_DATA_GATHER_COMPLETE 208

/**
 * Sent from pc to master, to set/get the current RTC date/time being served by the board.
 *
 * Request : Message(crc,<COMMAND>,<pc id>,<master id>,<new datetime>)
 * Response: Message(crc,<COMMAND_RESPONSE>,<master id>,<pc id>,<ok|error>)
 */
#define MASTER_RTC_TIME_GET            9
#define MASTER_RTC_DATE_GET           10
#define MASTER_RTC_TIME_SET           11
#define MASTER_RTC_DATE_SET           12
#define MASTER_RTC_TIME_GET_RESPONSE 109
#define MASTER_RTC_DATE_GET_RESPONSE 110
#define MASTER_RTC_TIME_SET_RESPONSE 111
#define MASTER_RTC_DATE_SET_RESPONSE 112

/**
 * Sent from pc to master, to set the id of the master device.
 *
 * Request : Message(crc,MASTER_ID_WRITE,<pc id>,<old master id>,<new master id>)
 * Response: Message(crc,MASTER_ID_WRITE_RESPONSE,<old master id>,<pc id>,<ok|error>)
 */
#define MASTER_ID_WRITE           13
#define MASTER_ID_WRITE_RESPONSE 113

/**
 * Sent from pc to master, to set/get the required parameter from the GPS device. Each command has
 * the description of the responses
 * Request : Message(crc,<COMMAND>,<pc id>,<master id>,<empty>)
 * Response: Message(crc,<COMMAND_RESPONSE>,<master id>,<pc id>,<m_content>)
 */

// Content should be set to empty.
#define MASTER_GPS_TIME_GET       14
#define MASTER_GPS_DATE_GET       15 //
#define MASTER_GPS_LAT_GET        16
#define MASTER_GPS_LON_GET        17
#define MASTER_GPS_ALTITUDE_GET   18
#define MASTER_GPS_FIX_TYPE_GET   19
#define MASTER_GPS_SPEED_GET      20
#define MASTER_GPS_TRACK_GET      21
#define MASTER_GPS_PDOP_GET       22
#define MASTER_GPS_HDOP_GET       23
#define MASTER_GPS_VDOP_GET       24
#define MASTER_GPS_ENABLED_GET    25

// Packing description bytes, starting at byte 0 of the m_content field.
#define MASTER_GPS_TIME_GET_RESPONSE      114 // hour_byte, minute_byte, second_byte
#define MASTER_GPS_DATE_GET_RESPONSE      115 // year_byte, month_byte, day_byte
#define MASTER_GPS_LAT_GET_RESPONSE       116 // integer_int32, decimal_int32
#define MASTER_GPS_LON_GET_RESPONSE       117 // integer_int32, decimal_int32
#define MASTER_GPS_ALTITUDE_GET_RESPONSE  118 // altitude_int32
#define MASTER_GPS_FIX_TYPE_GET_RESPONSE  119 // fixed_byte (0 or 1)
#define MASTER_GPS_SPEED_GET_RESPONSE     120 // integer_int32, decimal_int32
#define MASTER_GPS_TRACK_GET_RESPONSE     121 // tracking_byte
#define MASTER_GPS_PDOP_GET_RESPONSE      122 // integer_int32, decimal_int32
#define MASTER_GPS_HDOP_GET_RESPONSE      123 // integer_int32, decimal_int32
#define MASTER_GPS_VDOP_GET_RESPONSE      124 // integer_int32, decimal_int32
#define MASTER_GPS_ENABLED_GET_RESPONSE   125 // enabled_gps_byte


#define MASTER_BLE_ENABLED_SET             26
#define MASTER_BLE_ENABLED_GET             27
#define MASTER_BLE_ENABLED_SET_RESPONSE   126 // enabled_byte
#define MASTER_BLE_ENABLED_GET_RESPONSE   127 // enabled_byte

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
class Message {
public:
    uint16_t m_crc;
    byte m_type;
    //byte m_status;
    uint32_t m_fromId;
    uint32_t m_targetId;
    byte m_data[CUSTOM_MESSAGE_DATA_LENGTH];

    ~Message();
    size_t writeTo(DataStreamWriter *dsw);
    size_t readFrom(DataStreamReader *dsr);
    void clearData();
    void calculateAndSetCrc();
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

    uint32_t m_id;
    Handler *m_handler;

    /** Endpoint for debugging and error messages.*/
    Debugger d;

    void process(Message &message, DataStreamWriter *readFromLine, DataStreamWriter *opposingLine);
    void transmit(DataStreamWriter *dsw, Message &message);

public:
    DataExchanger();

    void setup(uint32_t id, Handler *handler, SerialEndpoint *debugEndpoint);
    void setupHardware(DataStreamReader *dsr, DataStreamWriter *dsw);
    void setupSoftware(DataStreamReader *dsr, DataStreamWriter *dsw);

    void loop();

    void injectMessage(Message &message);
};

#endif // __DATA_EXCHANGER_H_
