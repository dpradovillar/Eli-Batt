#ifndef __FIRMWARE_SLAVE_H_
#define __FIRMWARE_SLAVE_H_

#include <Endpoint.h>
#include <SimpleCrc.h>

class FirmwareSlave {
	Endpoint *e;
public:
    FirmwareSlave();
    void setup();
    void loop();
};

#endif // __FIRMWARE_SLAVE_H_
