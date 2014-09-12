#include <FirmwareSlave.h>

FirmwareSlave::FirmwareSlave() {
}

void FirmwareSlave::setup(int rx1, int tx1, int rx2, int tx2, int currentSensorPin,
        int voltageSensorPin, int bauds, Endpoint *debuggerEndpoint) {
    d.setup(debuggerEndpoint);

    d.println("Reading ID from EEPROM");
    m_eeprom_writer.read(m_id);
    d.print("ID read from EEPROM:").printInt(m_id).println();

    d.println("Setting First communication channel");
    m_dsr_a.setup(&m_comm_a);
    m_dsw_a.setup(&m_comm_a);
    m_comm_a.setup(rx1, tx1, bauds);
    m_comm_a.waitForConnection();

    d.println("Setting Second communication channel");
    m_dsr_b.setup(&m_comm_b);
    m_dsw_b.setup(&m_comm_b);
    m_comm_b.setup(rx2, tx2, bauds);
    m_comm_b.waitForConnection();

    d.println("Communications OK");

    d.println("Setting up communications");
    m_dex.setup(m_id, this);
    m_dex.setupHardware(&m_dsr_a, &m_dsw_a);
    m_dex.setupSoftware(&m_dsr_b, &m_dsw_b);

    d.println("Starting SLAVE tasks");
}

void FirmwareSlave::loop() {
    m_dex.loop();
}

bool FirmwareSlave::handleMessage(Message *message) {
    switch(message->m_type) {
    case TYPE_GET_ID: return getId(message);
    case TYPE_SET_ID: return setId(message);
    case TYPE_GET_DATA: return getData(message);
    }
    return false;
}

bool FirmwareSlave::getData(Message *message) {
    message->swapIds();

    // Encode a 3 valued vector into the message content.
    uint16_t digitalTemp = 0;
    uint16_t digitalCurrent = 0;
    uint16_t digitalVoltage = 0;

    // If the i2c sensor was properly set up.
    // TODO(rtapiapincheira): use an invalid value (as MIN_SHORT) as indicator of the sensor not
    // being correctly set up.
    if (m_temp_sensor.isAllSetUp()) {
        digitalTemp = m_temp_sensor.readDigital();
    }

    // For analog sensors, there's no general way of checking if the value read is reliable.
    // TODO(rtapiapincheira): make a mechanism for specific analog sensor checking if it's set up.
    digitalCurrent = m_current_sensor.read();
    digitalVoltage = m_voltage_sensor.read();

    // Fill the message content by: [SSTTCCVV] where of the 8 bytes:
    // SS: 2-bytes checksum of the following 6 bytes
    // TT: 2-bytes digital temperature
    // CC: 2-btyes digital current
    // VV: 2-bytes digital voltage
    Utils::putInMemory(message->m_data, 2, digitalTemp);
    Utils::putInMemory(message->m_data, 4, digitalCurrent);
    Utils::putInMemory(message->m_data, 6, digitalVoltage);

    // Checksum of the previously set 6 bytes.
    uint16_t crc16 = m_simple_crc.crc16(message->m_data+2, 6);
    // Finally put the checksum of the message.
    Utils::putInMemory(message->m_data, 0, crc16);

    return true; // Send it back as response!
}

bool FirmwareSlave::getId(Message *message) {
    message->swapIds(); // Response back to sender!

    // Put the id into first 4 bytes of the content of the message.
    byte id[4];
    m_eeprom_writer.read(id);
    Utils::copyArray(id, message->m_data, 4);

    return true;
}

bool FirmwareSlave::setId(Message *message) {
    // Write from message to eeprom.
    byte id[4];
    Utils::copyArray(message->m_data, id, 4);
    m_eeprom_writer.update(id);

    // No response!
    return false;
}
