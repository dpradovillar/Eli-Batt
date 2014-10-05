#include "threadedserial.h"

ThreadedSerial::ThreadedSerial() {
}

ThreadedSerial::~ThreadedSerial() {
}

void ThreadedSerial::run() {
    m_running = false;

    while(!m_running) {
        sleep(1);
        qDebug() << "HERE";
    }
}
