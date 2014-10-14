#include <FirmwareSlave.h>

FirmwareSlave::FirmwareSlave() {
}

void FirmwareSlave::setup(int rx1, int tx1, int bauds1, int rx2, int tx2, int bauds2,
        int currentSensorPin, int voltageSensorPin, int debugPin, SerialEndpoint *debuggerEndpoint)
{
    d.setup(debuggerEndpoint);
    Firmware::setup(rx1, tx1, bauds1, rx2, tx2, bauds2, currentSensorPin, voltageSensorPin,
            debugPin, NULL, debuggerEndpoint);
}

void FirmwareSlave::loop() {
    m_dex.loop();
}

bool FirmwareSlave::handleMessage(Message &message) {
    uint16_t temp;

    switch (message.m_type) {
    case SLAVE_ID_WRITE:
        d.println(F("SLAVE_ID_WRITE"));
        m_eeprom_writer.writeId(Utils::toInt32(message.m_data));

        message.clearData();
        message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
        message.m_type = SLAVE_ID_WRITE_RESPONSE;
        message.m_targetId = message.m_fromId;
        message.m_fromId = m_id;
        return true;
        break;
    case SLAVE_DATA_READ:
        d.println(F("SLAVE_DATA_READ"));

        Utils::toByte(temp = (uint16_t)m_temp_sensor.readDigital(), message.m_data);
        d.print(F("t=")).println(temp);

        Utils::toByte(temp = (uint16_t)m_current_sensor.read(), message.m_data+2);
        d.print(F("i=")).println(temp);

        Utils::toByte(temp = (uint16_t)m_voltage_sensor.read(), message.m_data+4);
        d.print(F("v=")).println(temp);

        message.m_type = SLAVE_DATA_READ_RESPONSE;
        // Swap ids.
        message.m_targetId = message.m_fromId;
        message.m_fromId = m_id;
        return true;
    default:
        break;
    }
    return false;
}
