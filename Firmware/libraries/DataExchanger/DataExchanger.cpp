#include <DataExchanger.h>

Message::~Message() {
}

size_t Message::writeTo(DataStreamWriter *dsw) {
    m_crc = calculateCrc();
    size_t r = 0;
    r += dsw->writeShort(m_crc);
    r += dsw->writeByte(m_type);
    r += dsw->writeByte(m_status);
    r += dsw->writeArray(m_fromId, ID_DATA_LENGTH);
    r += dsw->writeArray(m_targetId, ID_DATA_LENGTH);
    r += dsw->writeArray(m_data, CUSTOM_MESSAGE_DATA_LENGTH);
    if (r != MESSAGE_SIZE) {
        return -1;
    }
    return r;
}

size_t Message::readFrom(DataStreamReader *dsr) {
    bool ok = true;

    m_crc = dsr->readShort(&ok);
    size_t r = 2;
    if (!ok) return -1;

    m_type = dsr->readByte(&ok);
    r++;
    if (!ok) return -1;

    m_status = dsr->readByte(&ok);
    r++;
    if (!ok) return -1;

    dsr->readFully(m_fromId, ID_DATA_LENGTH, &ok);
    r += ID_DATA_LENGTH;
    if (!ok) return -1;

    dsr->readFully(m_targetId, ID_DATA_LENGTH, &ok);
    r += ID_DATA_LENGTH;
    if (!ok) return -1;

    dsr->readFully(m_data, CUSTOM_MESSAGE_DATA_LENGTH, &ok);
    r += CUSTOM_MESSAGE_DATA_LENGTH;
    if (!ok) return -1;

    if (r != MESSAGE_SIZE) {
        return -1;
    }
    return r;
}

uint16_t Message::calculateCrc() {
    byte buffer[MESSAGE_SIZE-2];
    buffer[0] = m_type;
    buffer[1] = m_status;
    Utils::copyArray(m_fromId, buffer+2, ID_DATA_LENGTH);
    Utils::copyArray(m_targetId, buffer+2+ID_DATA_LENGTH, ID_DATA_LENGTH);
    Utils::copyArray(m_data, buffer+2+2*ID_DATA_LENGTH, CUSTOM_MESSAGE_DATA_LENGTH);
    return SimpleCrc::crc16(buffer, MESSAGE_SIZE-2);
}

void Message::swapIds() {
    // Swap fromId and targetId arrays.
    byte buffer[ID_DATA_LENGTH];
    Utils::copyArray(m_fromId, buffer, ID_DATA_LENGTH);
    Utils::copyArray(m_targetId, m_fromId, ID_DATA_LENGTH);
    Utils::copyArray(buffer, m_targetId, ID_DATA_LENGTH);
}

Handler::~Handler() {
}

void DataExchanger::process(
        Message &message,                 // Message received.
        DataStreamWriter *readFromLine,   // Communication line where the message was read from.
        DataStreamWriter *opposingLine) { // Opposing communication line, to crosstalk messages.

	switch(message.m_type) {
	case SCAN_MESSAGE:
		// Transmit the same unaddressed scan message to the next in the chain.
		transmit(opposingLine, message);

		// Fall back to next case on purpose!

	case SCAN_ID_READ:
		// Put my id as the targetId.
		Utils::copyArray(m_id, message.m_targetId, ID_DATA_LENGTH);

		// Swap ids to send the message back (I'm sending a message addressed to master).
		message.swapIds();

		// Send back.
		transmit(readFromLine, message);

		break;

	case SCAN_ID_CHECK:
		// Put my id in the content of the message.
		Utils::copyArray(m_id, message.m_data, ID_DATA_LENGTH);

		// Swap ids to send back message (addressed to master).
		message.swapIds();

		// Send back.
		transmit(readFromLine, message);

		break;

	default:

		// Addressed to me?
		if (Utils::arrayEquals(message.m_targetId, m_id, ID_DATA_LENGTH)) {
			if (m_handler->handleMessage(message)) {
				transmit(readFromLine, message);
			}
		}
		// Not to me? pass it on.
		else {
			transmit(opposingLine, message);
		}
		break;
	}

}

void DataExchanger::transmit(DataStreamWriter *dsw, Message &message) {
    message.writeTo(dsw);
    dsw->flush();
}

DataExchanger::DataExchanger() :
        m_hardwareReader(NULL),
        m_hardwareWriter(NULL),
        m_softwareReader(NULL),
        m_softwareWriter(NULL),
        m_handler(NULL)
{}

void DataExchanger::setup(byte *id, Handler *handler, SerialEndpoint *debugEndpoint) {
    d.setup(debugEndpoint);

    Utils::copyArray(id, m_id, ID_DATA_LENGTH);
    m_handler = handler;
}

void DataExchanger::setupHardware(DataStreamReader *dsr, DataStreamWriter *dsw) {
    m_hardwareReader = dsr;
    m_hardwareWriter = dsw;
}

void DataExchanger::setupSoftware(DataStreamReader *dsr, DataStreamWriter *dsw) {
    m_softwareReader = dsr;
    m_softwareWriter = dsw;
}

void DataExchanger::loop() {
    Message m;
    if (m_hardwareReader && m_hardwareReader->available() >= MESSAGE_SIZE) {
        if (m.readFrom(m_hardwareReader) == -1) {
            d.println("ERROR while reading from hardware connection (comm A)");
        } else if (m_hardwareWriter && m_softwareWriter) {
            process(m, m_hardwareWriter, m_softwareWriter);
        } else {
        	d.println("WARNING package dismissed because there's no connection set up.");
        }
    }
    if (m_softwareReader && m_softwareReader->available() >= MESSAGE_SIZE) {
        if (m.readFrom(m_softwareReader) == -1) {
            d.println("ERROR while reading from software connection (comm B)");
        } else if (m_softwareWriter && m_hardwareWriter) {
            process(m, m_softwareWriter, m_hardwareWriter);
        } else {
        	d.println("WARNING package dismissed because there's no connection set up.");
        }
    }
}
