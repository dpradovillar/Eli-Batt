#ifndef __PARSER_COMM_H_
#define __PARSER_COMM_H_

#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Cmd.h>
#include <Utils.h>

#define USE_SOFT 1

#define MAX_SLAVES 10

#define MAX_LINE_BUFFER_SIZE 64

typedef struct {
    uint32_t id[MAX_SLAVES];
    float temperature[MAX_SLAVES];
    float current[MAX_SLAVES];
    float voltage[MAX_SLAVES];
    bool enabled[MAX_SLAVES];
    int n;
} SlaveStruct;

class ParserComm {
private:
#if USE_SOFT
    SoftwareSerial *m_bank;
#else
    HardwareSerial *m_bank;
#endif

    int getIndexFor(uint32_t id);

public:
    SlaveStruct slaves;

    ParserComm();
    ~ParserComm();

    void setup(
#if USE_SOFT
        SoftwareSerial *s
#else
        HardwareSerial *s
#endif
    );

    void putMasterInfo(uint32_t id, float temp, float current, float voltage);
    void putSlaveInfo(uint32_t id, float temp, float current, float voltage);

    void processLine(char *s, int len);

    void loop();
};

#endif // __PARSER_COMM_H_
