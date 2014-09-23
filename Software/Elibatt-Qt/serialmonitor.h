#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QtCore>
#include <QComboBox>
#include <QSerialPortInfo>

/**
 * @brief The SerialMonitor class scans periodically for new serial devices and updates a widget.
 */
class SerialMonitor : public QObject
{
    Q_OBJECT

    QStringList m_lastDevices;
    QComboBox *m_combobox;
    QTimer m_timer;

    bool m_firstTimeout;

    /**
     * Returns true if the toSelect portname is in the new list (if the serial connection is open,
     * it should keep it that way). If false, the connection should be closed because the serial
     * port is no longer connected to the PC).*/
    bool replaceItems(const QString &toSelect, const QStringList &list);

    /**
     * Returns a list of the port names of the devices currently connected to the PC.
     */
    QStringList currentDeviceNames();

public:
    SerialMonitor();

    void setComboBox(QComboBox *combobox);

    void start();
    void stop();

private slots:
    void onTimeout();

signals:
    void onSerialListChanged(QStringList newOnes, bool forceDisconnect);
};

#endif // SERIALMONITOR_H
