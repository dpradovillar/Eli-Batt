#include "serialmonitor.h"

bool SerialMonitor::replaceItems(const QString &toSelect, const QStringList &list) {
    m_combobox->clear();
    int index = -1;
    int i = 0;
    foreach(QString s, list) {
        m_combobox->addItem(s);
        if (s.compare(toSelect, Qt::CaseInsensitive) == 0) {
            index = i;
        }
        i++;
    }
    if (index >= 0) {
        m_combobox->setCurrentIndex(index);
        return true;
    }
    return false;
}

QStringList SerialMonitor::currentDeviceNames () {
    QList<QSerialPortInfo> availableSerials = QSerialPortInfo::availablePorts();

    QStringList newPortnames;
    foreach (const QSerialPortInfo &info, availableSerials) {
        newPortnames.append(info.portName());
    }
    newPortnames.sort();
    return newPortnames;
}

SerialMonitor::SerialMonitor() {
    m_timer.setInterval(300);
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void SerialMonitor::start() {
    m_firstTimeout = false;
    m_timer.start();

    onSerialListChanged(QStringList(), false);
}

void SerialMonitor::stop() {
    m_firstTimeout = true;
    m_timer.stop();
}

void SerialMonitor::setComboBox(QComboBox *combobox) {
    m_combobox = combobox;
}

void SerialMonitor::onTimeout() {
    QString port = m_combobox->currentText();
    QStringList availableSerials = currentDeviceNames();

    if (availableSerials != m_lastDevices) {
        m_lastDevices = availableSerials;
        bool shouldDisconnect = !replaceItems(port, m_lastDevices);
        emit onSerialListChanged(m_lastDevices, shouldDisconnect);
    }
}
