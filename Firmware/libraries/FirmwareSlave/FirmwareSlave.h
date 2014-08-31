#ifndef __FIRMWARE_SLAVE_H_
#define __FIRMWARE_SLAVE_H_
/*
#include <SimpleCrc.h>
#include <EepromId.h>
#include <SampleCollector.h>
*/
class FirmwareSlave {
public:
	FirmwareSlave();
	void setup();
	void loop();
};

#endif // __FIRMWARE_SLAVE_H_
