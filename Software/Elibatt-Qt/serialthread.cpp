#include "serialthread.h"

SerialThread::SerialThread(QObject *parent) :
    QThread(parent),
    m_portName(""),
    m_bauds(9600),
    m_serialPort(NULL),
    m_quit(false)
{
}

SerialThread::~SerialThread()
{
    QMutexLocker locker(&m_mutex);
    m_quit = true;
}

void SerialThread::setParams(const QString &portName, quint32 bauds) {
    m_portName = portName;
    m_bauds = bauds;

    qDebug() << "Setting params, portName:" << portName << ", bauds:" << bauds;
}

void SerialThread::transmitPackage(const Message &msg)
{
    QMutexLocker locker(&m_mutex);
    qDebug() << "Transmiting new msg";
    m_messages.append(msg);
}

void SerialThread::stop() {
    qDebug() << "Stopping thread!";

    m_quit = true;
}

void SerialThread::run() {
    qDebug() << "Opening new connection";

    m_serialPort = new QSerialPort;
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
    if ( ! m_serialPort->open(QIODevice::ReadWrite)) {
        emit newError(tr("Can't open %1, error code %2").arg(m_portName).arg(m_serialPort->error()));
        return;
    }

    qDebug() << "Connection open on port:" << m_portName << ", bauds:" << m_bauds;

    while (!m_quit) {
        Message nextMessage;
        bool hasNext = false;
        {
            QMutexLocker ml(&m_mutex);
            if (!m_messages.isEmpty()) {
                nextMessage = m_messages.takeFirst();
                hasNext = true;
            }
        }

        qDebug() << "Scheduling sending of a new message?" << hasNext;

        qDebug() << "Waiting a bit for any incoming message...";
        // read response
        if (m_serialPort->waitForReadyRead(WAIT_SWITCH_TIMEOUT)) {

            qDebug() << "Bytes detected, waiting for the remaining of the bytes";

            // Read all the messages available first!
            while (m_serialPort->bytesAvailable() > 0) {
                uchar buffer[MESSAGE_SIZE];
                int read = 0;
                while (read < MESSAGE_SIZE){
                    uchar c = 0;
                    int r = m_serialPort->read((char*)&c, 1);
                    read += r;
                    qDebug() << "current read:" << read << ", c:" << (int)c << ", r:" << r;
                }
                qDebug() << "Buffer complete, emitting new message";
                Message m;
                m.readFrom(buffer);
                emit newResponse(m);
            }
        } else {
            /*emit timeout(tr("Wait read response timeout %1")
                         .arg(QTime::currentTime().toString()));*/
            qDebug() << "TIMEOUT while receiving new packages";

            if (hasNext) {
                qDebug() << "Sending next message to device!";

                uchar buffer[MESSAGE_SIZE];
                nextMessage.serializeTo(buffer);
                int written = 0;
                while(written < MESSAGE_SIZE) {
                    written += m_serialPort->write((char*)(buffer+written), MESSAGE_SIZE-written);
                    if ( ! m_serialPort->waitForBytesWritten(WAIT_WRITE_TIMEOUT)) {

                        qDebug() << "Writing operation timed out, Message is LOST";

                        emit newError("Writing operation timed out, Message is LOST");
                        break;
                    }
                }

                qDebug() << "All bytes of the scheduled message were sent!";
            }
        }
    }

    m_serialPort->close();
    delete m_serialPort;
    m_serialPort = NULL;
}
