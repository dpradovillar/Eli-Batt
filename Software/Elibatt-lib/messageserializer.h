#ifndef MESSAGESERIALIZER_H
#define MESSAGESERIALIZER_H

#include <QtCore>
#include <QDebug>
#include <QByteArray>

class Message;

#define CUSTOM_MESSAGE_DATA_LENGTH  8
#define MESSAGE_SIZE               19

class MessageSerializer : public QObject
{
    Q_OBJECT
public:
    explicit MessageSerializer();
    virtual ~MessageSerializer();
};

class Message {
public:
    quint16 m_crc;                            // 2
    uchar m_type;                             // 1
    quint32 m_fromId;                         // 4
    quint32 m_targetId;                       // 4
    uchar m_data[CUSTOM_MESSAGE_DATA_LENGTH]; // 8

    void serializeTo(uchar *buff19bytes);     // MESSAGE_SIZE buffer
    void readFrom(uchar *buff19bytes);        // MESSAGE_SIZE buffer

    QString toString() const;
};

#endif // MESSAGESERIALIZER_H
