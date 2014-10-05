#ifndef SERIALLIB_H
#define SERIALLIB_H

#include <QDateTime>
#include <QDebug>
#include <QSerialPort>
#include <QTimer>
#include <QtCore>

#include <iostream>
using namespace std;

/**
 * @brief The SerialLib class encapsulates a serial connection and allows easy transfer/read of
 * bytes. This is a wrapper of Qt serial connection. All read and write methods should be called
 * on the same thread this object was created, as that's a restriction of the Qt framework.
 */
class SerialLib : public QObject {
  Q_OBJECT
private:
    QSerialPort m_serialPort;

public:
    /**
     * @brief SerialLib builds an empty, unopened, SerialLib.
     */
    explicit SerialLib();

    /**
     * @brief ~SerialLib common practice to force subclasses to execute "cleaning" code defined
     * in this class.
     */
    virtual ~SerialLib();

    /**
     * @brief begin tries to open the serial connection, given the port name.
     * @param portName name of the system port to be opened (depends on the operating system).
     * @param baud speed at which the connection will be stablished (if no error ocurrs).
     * @return true if connection is open successfully, false otherwise.
     */
    bool begin(const QString &portName, qint32 bauds);

    /**
     * @brief available returns how many bytes are stored in the internal buffer of this object.
     * @return non-negative number of bytes available.
     */
    int available();

    /**
     * @brief read pops a single byte value from the underlying stream.
     * @return a value in the range 0,255 with the correct byte, or -1 if an error ocurred.
     */
    int read();

    /**
     * @brief write transfer a single byte value over the underlying stream.
     * @param c byte to be transferred along the stream.
     * @return  true on success, false on error (stream closed, stream error, unexpected error, etc).
     */
    bool write(uchar c);
};

#endif // SERIALLIB_H
