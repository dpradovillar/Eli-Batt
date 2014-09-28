#include <FirmwareSlave.h>

FirmwareSlave::FirmwareSlave() {
}

void FirmwareSlave::setup(int rx1, int tx1, int bauds1, int rx2, int tx2, int bauds2,
        int currentSensorPin, int voltageSensorPin, int debugPin, SerialEndpoint *debuggerEndpoint)
{
    d.setup(debuggerEndpoint);
    Firmware::setup(rx1, tx1, bauds1, rx2, tx2, bauds2, currentSensorPin, voltageSensorPin,
    		debugPin, debuggerEndpoint);
}

void FirmwareSlave::loop() {
    m_dex.loop();
}

bool FirmwareSlave::handleMessage(Message &message) {
	switch (message.m_type) {
	case SLAVE_ID_WRITE:
		break;
	case SLAVE_DATA_READ:
		break;
	default:
		break;
	}
    return true;
}
