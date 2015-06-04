#ifndef __V2_LIBS_COMM_H_
#define __V2_LIBS_COMM_H_

#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Cmd.h>

#define BUFFER_MAX   100

class V2LibsComm {
private:
    HardwareSerial *m_hard;
    SoftwareSerial *m_soft;
    int m_last_command;
    char cmdBuffer[BUFFER_MAX];
    int cmdLen;

    bool m_waiting_closing;
    Cmd cmd;

public:
    V2LibsComm();
    ~V2LibsComm();

    void setup(HardwareSerial *h, SoftwareSerial *s);
    void loop();

    int getCommand();
    int takeCommand();
};

#endif // __V2_LIBS_COMM_H_
