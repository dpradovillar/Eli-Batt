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
    uint16_t temp;

    switch (message.m_type) {
    case SLAVE_ID_WRITE:
        break;
    case SLAVE_DATA_READ:
        d.println("SLAVE_DATA_READ");

        d.println("MASTER_DATA_READ:");
        Utils::toByte(temp = (uint16_t)m_temp_sensor.readDigital(), message.m_data);
        d.print("t=").println(temp);

        Utils::toByte(temp = (uint16_t)m_current_sensor.read(), message.m_data+2);
        d.print("i=").println(temp);

        Utils::toByte(temp = (uint16_t)m_voltage_sensor.read(), message.m_data+4);
        d.print("v=").println(temp);

        message.m_type = SLAVE_DATA_READ_RESPONSE;
        break;
    default:
        break;
    }
    return false;
}
