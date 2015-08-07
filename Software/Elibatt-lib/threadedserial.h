#ifndef THREADEDSERIAL_H
#define THREADEDSERIAL_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
#include <QTime>

#include "messageserializer.h"

#define WAIT_SWITCH_TIMEOUT 1000 // Time before to switch to writing
#define WAIT_WRITE_TIMEOUT   500 // If the device is not reading my bytes, wait this
#define WAIT_READ_TIMEOUT    100

#define ENABLE_SERIAL_COMM

class ThreadedSerial : public QThread {
    Q_OBJECT

public:
    ThreadedSerial(QObject *parent = 0);
    ~ThreadedSerial();

    void setParams(const QString &portName, quint32 bauds);
    void transmitPackage(const Message &msg);
    void stop();
    void run();

signals:
    void newResponse(const Message &msg);
    void newError(const QString &s);
    void newTimeout(const QString &s);

private:
    QString m_portName;
    quint32 m_bauds;
    QSerialPort *m_serialPort;

    QMutex m_mutex;
    QList<Message> m_messages;
    bool m_quit;
};

#endif // THREADEDSERIAL_H
