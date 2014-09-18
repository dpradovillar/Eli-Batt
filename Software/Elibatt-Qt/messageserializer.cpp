#include "messageserializer.h"

MessageSerializer::MessageSerializer()
{
}

MessageSerializer::~MessageSerializer() {

}

void Message::serializeTo(uchar *buff20bytes) {
    // CRC
    buff20bytes[0] = (uchar)(m_crc >> 8);
    buff20bytes[1] = (uchar)(m_crc >> 0);

    // TYPE
    buff20bytes[2] = m_type;

    // STATUS
    buff20bytes[3] = m_status;

    // FROM ID
    buff20bytes[4] = m_fromId[0];
    buff20bytes[5] = m_fromId[1];
    buff20bytes[6] = m_fromId[2];
    buff20bytes[7] = m_fromId[3];

    // TARGET ID
    buff20bytes[8] = m_targetId[0];
    buff20bytes[9] = m_targetId[1];
    buff20bytes[10] = m_targetId[2];
    buff20bytes[11] = m_targetId[3];

    // DATA
    buff20bytes[12] = m_data[0];
    buff20bytes[13] = m_data[1];
    buff20bytes[14] = m_data[2];
    buff20bytes[15] = m_data[3];

    buff20bytes[16] = m_data[4];
    buff20bytes[17] = m_data[5];
    buff20bytes[18] = m_data[6];
    buff20bytes[19] = m_data[7];
}

void Message::readFrom(uchar *buff20bytes) {
    // CRC
    m_crc = (((quint16)buff20bytes[0]) << 8) | (((quint16)buff20bytes[1]) << 0);

    // TYPE
    m_type = buff20bytes[2];

    // STATUS
    m_status = buff20bytes[3];

    // FROM ID
    m_fromId[0] = buff20bytes[4];
    m_fromId[1] = buff20bytes[5];
    m_fromId[2] = buff20bytes[6];
    m_fromId[3] = buff20bytes[7];

    // TARGET ID
    m_targetId[0] = buff20bytes[8];
    m_targetId[1] = buff20bytes[9];
    m_targetId[2] = buff20bytes[10];
    m_targetId[3] = buff20bytes[11];

    // DATA
    m_data[0] = buff20bytes[12];
    m_data[1] = buff20bytes[13];
    m_data[2] = buff20bytes[14];
    m_data[3] = buff20bytes[15];

    m_data[4] = buff20bytes[16];
    m_data[5] = buff20bytes[17];
    m_data[6] = buff20bytes[18];
    m_data[7] = buff20bytes[19];
}

QString Message::toString() const {
    QString s = QString("crc:%1, type:%2, status: %3")
            .arg((int)m_crc)
            .arg((int)m_type)
            .arg((int)m_status);
    s += ", fromId:";
    for(int i=0; i<4; i++) s += (" "+QString::number((int)m_fromId[i]));

    s += ", targetId:";
    for(int i=0; i<4; i++) s += (" "+QString::number((int)m_targetId[i]));

    s += ", data:";
    for(int i=0; i<8; i++) s += (" "+QString::number((int)m_data[i]));

    return s;
}

