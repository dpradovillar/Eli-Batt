#include "messageserializer.h"

MessageSerializer::MessageSerializer()
{
}

MessageSerializer::~MessageSerializer() {
}

void Message::serializeTo(uchar *buff19bytes) {
    // CRC
    buff19bytes[0] = (uchar)(m_crc >> 8);
    buff19bytes[1] = (uchar)(m_crc >> 0);

    // TYPE
    buff19bytes[2] = m_type;

    // FROM ID
    buff19bytes[3] = (uchar)(m_fromId >> 24);
    buff19bytes[4] = (uchar)(m_fromId >> 16);
    buff19bytes[5] = (uchar)(m_fromId >>  8);
    buff19bytes[6] = (uchar)(m_fromId >>  0);

    // TARGET ID
    buff19bytes[7]  = (uchar)(m_targetId >> 24);
    buff19bytes[8]  = (uchar)(m_targetId >> 16);
    buff19bytes[9]  = (uchar)(m_targetId >>  8);
    buff19bytes[10] = (uchar)(m_targetId >>  0);

    // DATA
    buff19bytes[11] = m_data[0];
    buff19bytes[12] = m_data[1];
    buff19bytes[13] = m_data[2];
    buff19bytes[14] = m_data[3];

    buff19bytes[15] = m_data[4];
    buff19bytes[16] = m_data[5];
    buff19bytes[17] = m_data[6];
    buff19bytes[18] = m_data[7];
}

void Message::readFrom(uchar *buff19bytes) {
    // CRC
    m_crc =
            (((quint16)buff19bytes[0]) << 8) |
            (((quint16)buff19bytes[1]) << 0);

    // TYPE
    m_type = buff19bytes[2];

    // FROM ID
    m_fromId =
            (((quint32)buff19bytes[3]) << 24) |
            (((quint32)buff19bytes[4]) << 16) |
            (((quint32)buff19bytes[5]) << 8) |
            (((quint32)buff19bytes[6]) << 0);

    // TARGET ID
    m_targetId =
            (((quint32)buff19bytes[7]) << 24) |
            (((quint32)buff19bytes[8]) << 16) |
            (((quint32)buff19bytes[9]) << 8) |
            (((quint32)buff19bytes[10]) << 0);

    // DATA
    m_data[0] = buff19bytes[11];
    m_data[1] = buff19bytes[12];
    m_data[2] = buff19bytes[13];
    m_data[3] = buff19bytes[14];

    m_data[4] = buff19bytes[15];
    m_data[5] = buff19bytes[16];
    m_data[6] = buff19bytes[17];
    m_data[7] = buff19bytes[18];
}

QString Message::toString() const {
    QString s = QString("crc:%1, type:%2, from:%3, target:%4")
            .arg((int)m_crc)
            .arg((int)m_type)
            .arg((int)m_fromId)
            .arg((int)m_targetId);

    if (m_type == 255) {
        s += ", data:";
        s += QString(QByteArray((const char*)m_data, 8));
    } else {
        s += ", data:";
        for(int i=0; i<8; i++) s += (" "+QString::number((int)m_data[i]));
    }

    return s;
}

