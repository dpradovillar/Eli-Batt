#include "seriallib.h"

SerialLib::SerialLib() {
}

SerialLib::~SerialLib() {
}

bool SerialLib::begin(const QString &portName, qint32 bauds) {
    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(bauds);
    cout << "opening port:" << portName.toStdString() << ", bauds:" << bauds << endl;
    return false;
}

int SerialLib::available() {
    return 0;
}

int SerialLib::read() {
    return -1;
}

bool SerialLib::write(uchar c) {
    return false;
}
