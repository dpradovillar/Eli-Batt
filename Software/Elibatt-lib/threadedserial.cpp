#include "threadedserial.h"

//#define DEBUG
#define PACKED_OUTPUT


ThreadedSerial::ThreadedSerial(QObject *parent) :
    QThread(parent),
    m_portName(""),
    m_bauds(9600),
    m_serialPort(NULL),
    m_quit(false)
{
}

ThreadedSerial::~ThreadedSerial()
{
    QMutexLocker locker(&m_mutex);
    m_quit = true;
}

void ThreadedSerial::setParams(const QString &portName, quint32 bauds) {
    m_portName = portName;
    m_bauds = bauds;

#ifdef DEBUG
    qDebug() << "Setting params, portName:" << portName << ", bauds:" << bauds;
#endif
}

void ThreadedSerial::transmitPackage(const Message &msg)
{
    QMutexLocker locker(&m_mutex);
#ifdef DEBUG
    qDebug() << "Transmiting new message";
#endif
    m_messages.append(msg);
}

void ThreadedSerial::stop() {
#ifdef DEBUG
    qDebug() << "Stopping thread!";
#endif
    m_quit = true;
}

void ThreadedSerial::run() {
#ifdef DEBUG
    qDebug() << "Opening new connection";
#endif

    m_serialPort = new QSerialPort;
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(m_bauds, QSerialPort::AllDirections);
    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit newError(tr("Can't open %1, error code %2").arg(m_portName).arg(m_serialPort->error()));
        return;
    }
#ifdef DEBUG
    qDebug() << "Connection open on port:" << m_portName << ", bauds:" << m_bauds;
#endif

    uchar buffer[MESSAGE_SIZE];
    int read = 0;

    while (!m_quit) {

#ifdef ENABLE_SERIAL_COMM
#ifdef DEBUG
        //qDebug() << "Scheduling sending of a new message?" << hasNext;

        //qDebug() << "Waiting a bit for any incoming message...";
#endif

#ifdef PACKED_OUTPUT
        // read response
        if (m_serialPort->waitForReadyRead(WAIT_SWITCH_TIMEOUT)) {

#ifdef DEBUG
            qDebug() << "Bytes detected, waiting for the remaining of the bytes";
#endif

            // Read all the messages available first!
            int available;
            while ( (available = m_serialPort->bytesAvailable()) > 0 && read<MESSAGE_SIZE) {

#ifdef DEBUG
                qDebug() << "available to read:" << available;
#endif

                while (read < MESSAGE_SIZE){
                    int r = m_serialPort->read((char*)(buffer+read), MESSAGE_SIZE-read);
                    if (r == 0) {
                        if (!m_serialPort->waitForReadyRead(WAIT_READ_TIMEOUT)) {

                        }
                        //break;
                    }
                    read += r;
#ifdef DEBUG
                    //qDebug() << "r:" << r << ", read:" << read;
                    //qDebug() << "current read:" << read << ", c:" << (int)(*(buffer+read)) << ", r:" << r;
#endif
                }

                if (read >= MESSAGE_SIZE) {
#ifdef DEBUG
                    qDebug() << "Buffer complete, emitting new message";
#endif

#ifdef DEBUG
                    m_mutex.lock();
                    qDebug() << "packages remaining:" << m_messages.size();
                    m_mutex.unlock();
#endif

                    Message m;
                    m.readFrom(buffer);
                    qDebug() << "New buffer read:" << m.toString();
                    emit newResponse(m);

                    read = 0;
                }
            }
        }
#else
        if (m_serialPort->waitForReadyRead(WAIT_SWITCH_TIMEOUT)) {
            int available;
            while ((available = m_serialPort->bytesAvailable()) > 0) {
                for(int i=0; i<available; i++) {
                    qDebug() << QString(m_serialPort->readAll()).toLatin1();
                }
            }
        }

#endif
        else {
#ifdef DEBUG
            //qDebug() << "TIMEOUT while receiving new packages";
#endif

            Message nextMessage;
            bool hasNext = false;
            {
                QMutexLocker ml(&m_mutex);
                if (!m_messages.isEmpty()) {
                    nextMessage = m_messages.takeFirst();
                    hasNext = true;
#ifdef DEBUG
                    //qDebug() << "messages remaining:" << m_messages.size();
#endif
                }
            }

            if (hasNext) {
                qDebug() << "Sending next message to device! " << nextMessage.toString();

                uchar buffer[MESSAGE_SIZE];
                nextMessage.serializeTo(buffer);
                int written = 0;
                while(written < MESSAGE_SIZE) {
                    written += m_serialPort->write((char*)(buffer+written), 1);
                    //msleep(1);
                    //qDebug() << "written:" << written;
                    if (!m_serialPort->waitForBytesWritten(WAIT_WRITE_TIMEOUT)) {
                        qDebug() << "Writing operation timed out, Message is LOST";
                        emit newError("Writing operation timed out, Message is LOST");
                        break;
                    }
                }
                msleep(5);
#ifdef DEBUG
                qDebug() << "All bytes of the scheduled message were sent!";
                qDebug() << "Message sent:" << nextMessage.toString();
#endif
            }
        }
#else
        qDebug() << "Skipping serial comm handling";
        // No packages are coming, simulate a timed out read.
        msleep(WAIT_SWITCH_TIMEOUT);
#endif
    }

    m_serialPort->close();
    delete m_serialPort;
    m_serialPort = NULL;
}
