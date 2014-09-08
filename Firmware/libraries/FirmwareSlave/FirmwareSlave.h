#ifndef __FIRMWARE_SLAVE_H_
#define __FIRMWARE_SLAVE_H_

class FirmwareSlave {
public:
    FirmwareSlave();
    void setup();
    void loop();
/*
    void printId(SerialEndpoint *serialEndpoint);
    void printConnectionToPcOk();*/
};

#endif // __FIRMWARE_SLAVE_H_
