#include <DataExchanger.h>



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

    r += dsr->readFully(m_fromId, ID_DATA_LENGTH, &ok);
    if (!ok) return -1;

    r += dsr->readFully(m_targetId, ID_DATA_LENGTH, &ok);
    if (!ok) return -1;

    r += dsr->readFully(m_data, CUSTOM_MESSAGE_DATA_LENGTH, &ok);
    if (!ok) return -1;

    if (r != MESSAGE_SIZE) {
        return -1;
    }
    return r;
}

uint16_t Message::calculateCrc() {
    byte buffer[MESSAGE_SIZE];
    buffer[0] = (byte)(m_crc >> 8);
    buffer[1] = (byte)(m_crc >> 0);
    buffer[2] = m_type;
    buffer[3] = m_status;
    copyArray(m_fromId, buffer+4, ID_DATA_LENGTH);
    copyArray(m_targetId, buffer+4+ID_DATA_LENGTH, ID_DATA_LENGTH);
    copyArray(m_data, buffer+4+2*ID_DATA_LENGTH, CUSTOM_MESSAGE_DATA_LENGTH);
    return SimpleCrc::crc16(buffer, MESSAGE_SIZE);
}

void DataExchanger::processHardware(Message *message) {
    if (message->m_type == TYPE_PING) {
        copyArray()
        message->m_targetId
    }
}
void DataExchanger::processSoftware(Message *message) {

}

DataExchanger::DataExchanger() :
        m_hardwareReader(NULL),
        m_hardwareWriter(NULL),
        m_softwareReader(NULL),
        m_softwareWriter(NULL)
{}

void DataExchanger::setupHardware(DataStreamReader *dsr, DataStreamWriter *dsw) {
    m_hardwareReader = dsr;
    m_hardwareWriter = dsw;
}

void DataExchanger::setupSoftware(DataStreamReader *dsr, DataStreamWriter *dsw) {
    m_softwareReader = dsr;
    m_softwareWriter = dsw;
}

void DataExchanger::loop() {
    bool ok = true;
    Message m;
    if (m_hardwareReader && m_hardwareReader->available() >= MESSAGE_SIZE) {
        m_hardwareReader->readObject(&m, &ok);
        if (ok) {
            processHardware(&m);
        } else {
            // TODO(rtapiapincheira): handle this error!
            // do nothing...
        }
    }
    if (m_softwareReader && m_softwareReader->available() >= MESSAGE_SIZE) {
        m_softwareReader->readObject(&m, &ok);
        if (ok) {
            processSoftware(&m);
        } else {
            // TODO(rtapiapincheira): handle this error!
            // do nothing...
        }
    }
}
