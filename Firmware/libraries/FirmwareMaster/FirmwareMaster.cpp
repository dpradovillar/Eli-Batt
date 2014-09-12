#include <FirmwareMaster.h>

FirmwareMaster::FirmwareMaster() {
}

void FirmwareMaster::setup(int rx2, int tx2, int bauds,
        Endpoint *dbgEndpoint) {
    d.setup(dbgEndpoint);

    d.println("Reading ID from EEPROM");
    m_eeprom_writer.read(m_id);
    d.print("ID read from EEPROM: ").printInt(m_id).println();

    d.println("Setting First communication channel to NULL");
    m_dsr_a.setup(&m_comm_a);
	m_dsw_a.setup(&m_comm_a);
    m_comm_a.setup(-1, -1, bauds); // The master device has only one serial connection
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

    d.println("Starting MASTER tasks");
}

void FirmwareMaster::loop() {
    m_dex.loop();
}

bool FirmwareMaster::handleMessage(Message *message) {
// The Master device handler should use the values (generally only) when the type is TYPE_GET_DATA
// and log them into the SD card or spit them out to a PC serial communication.

// Print it out to pc comm.
m_soh.handleMessage(message);

// TODO(rtapiapincheira): delete these debugging lines
if (message->m_type == TYPE_GET_DATA) {

    // TODO(rtapiapincheira): decode positions 0 and 1 as the content CRC and verify if they're
    // correct.

    uint16_t val1 = Utils::toShort(message->m_data[2], message->m_data[3]);
    uint16_t val2 = Utils::toShort(message->m_data[4], message->m_data[5]);
    uint16_t val3 = Utils::toShort(message->m_data[6], message->m_data[7]);

    d
    .print("temperature=").print(val1) // TOOD(rtapiapincheira): convert this value from digital to celsius
    .print(";current=").print(val2)
    .print(";voltage=").print(val3)
    .println();
}

    return false;
}

// Uppercase commands are targeted to MASTER and lowercase to SLAVE ones.
void FirmwareMaster::process(int next, Endpoint *endpoint) {
    d.print("Processing next:");
    d.println(next);

    byte targetId[4];

    switch(next) {
    case 'W': /* TODO: write eeprom id of the master device */ break;
    case 'R': /* TODO: read eeprom id of the master device */ break;
    case 'D': /* TODO: read gps, temp, etc from master and send them back! */ break;

    case 'w':
        // Read the next 8 bytes.
        byte oldNew[8];
        while(endpoint->available() < 8);
        endpoint->read(oldNew, 8);

        // Send message to existing slave, to make it change its own eeprom id.
        requestWriteId(oldNew, oldNew+4);
        break;

    case 'r':
        // Read the next 4 bytes.
        while(endpoint->available() < 4);
        endpoint->read(targetId, 4);

        // Send message to existing slave, to make it return its eeprom id. Strictly speaking, we
        // already know its eeprom id, so instead, we will query for a checksum of its id, and we'll
        // validate if it's correct (though, validating both communication and checksum construction
        // correctness). Regardless of this, when the slave is connected directly into a PC, the 'r'
        // command can be used to get the unknown eeprom id of that board.
        requestReadId(targetId);
        break;

    case 'd':
        // Read the next 4 bytes.
        while(endpoint->available() < 4);
        endpoint->read(targetId, 4);

        requestData(targetId);
        break;
    }
}

void FirmwareMaster::requestWriteId(byte *targetId, byte *newId) {
    m_the_message.m_type = TYPE_SET_ID;
    m_the_message.m_status = 0;
    Utils::copyArray(m_id, m_the_message.m_fromId, 4);
    Utils::copyArray(targetId, m_the_message.m_targetId, 4);
    Utils::copyArray(m_the_message.m_data, newId, 4);
    m_the_message.m_crc = m_the_message.calculateCrc();

    Serial.println("Message being sent:");
    m_soh.handleMessage(&m_the_message);

    Serial.println("Sending message through both endpoints");
    m_dsw_a.writeObject(&m_the_message); // Automatic flush!
    m_dsw_b.writeObject(&m_the_message); // Automatic flush!
}

void FirmwareMaster::requestReadId(byte *targetId) {
    m_the_message.m_type = TYPE_GET_ID;
    m_the_message.m_status = 0;
    Utils::copyArray(m_id, m_the_message.m_fromId, 4);
    Utils::copyArray(targetId, m_the_message.m_targetId, 4);
    // Set all to 0 to ensure the crc is calculated deterministically!
    Utils::copyArray("\0\0\0\0\0\0\0\0", m_the_message.m_data, CUSTOM_MESSAGE_DATA_LENGTH);
    m_the_message.m_crc = m_the_message.calculateCrc();

    Serial.println("Message being sent:");
m_soh.handleMessage(&m_the_message);

    Serial.println("Sending message through both endpoints");
    m_dsw_a.writeObject(&m_the_message); // Automatic flush!
    m_dsw_b.writeObject(&m_the_message); // Automatic flush!
}

void FirmwareMaster::requestData(byte *targetId) {
    m_the_message.m_type = TYPE_GET_DATA;
    m_the_message.m_status = 0;
    Utils::copyArray(m_id, m_the_message.m_fromId, 4);
    Utils::copyArray(targetId, m_the_message.m_targetId, 4);
    // Set all to 0 to ensure the crc is calculated deterministically!
    Utils::copyArray("\0\0\0\0\0\0\0\0", m_the_message.m_data, CUSTOM_MESSAGE_DATA_LENGTH);
    m_the_message.m_crc = m_the_message.calculateCrc();

    Serial.println("Message being sent:");
m_soh.handleMessage(&m_the_message);

    Serial.println("Sending message through both endpoints");
    m_dsw_a.writeObject(&m_the_message); // Automatic flush!
    m_dsw_b.writeObject(&m_the_message); // Automatic flush!
}

