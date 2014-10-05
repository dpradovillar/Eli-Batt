#ifndef THREADEDSERIAL_H
#define THREADEDSERIAL_H

#include <QDateTime>
#include <QDebug>
#include <QSerialPort>
#include <QTimer>
#include <QtCore>

#include <iostream>
using namespace std;

/**
 * @brief The ThreadedSerial class contains an internal SerialLib object to perform
 * I/O operations
 */
class ThreadedSerial : public QThread {
  Q_OBJECT
private:
    QSerialPort m_serialPort;
    bool m_running;

public:
    explicit ThreadedSerial();
    virtual ~ThreadedSerial();

protected:
    void run();
};

#endif // THREADEDSERIAL_H
