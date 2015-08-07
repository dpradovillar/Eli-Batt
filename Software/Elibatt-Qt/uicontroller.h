#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "ui_uidialog.h"
#include "serialmonitor.h"
#include "connectionstatus.h"
#include "eepromcontroller.h"
#include "gpscontroller.h"
#include "serialthread.h"

class UiController : public QObject {
    Q_OBJECT

private:
    Ui::UiDialog *m_ui;
    SerialMonitor m_serialMonitor;
    ConnectionStatus m_connectionStatus;
    EepromController m_eepromController;
    GpsController m_gpsController;
    SerialThread *m_serialThread;

public:
    explicit UiController();
    virtual ~UiController();

    void start();
    void stop();

    void setUi(Ui::UiDialog *ui);

private slots:
    void handleSerialListChanged(const QStringList &, bool forceDisconnect);
    void handleOnlineTimerTimeout(quint64 diff);
    void handleConnectButtonClicked();

private:
    void setEnabledParams(bool state);
    void setEnabledConnect(bool state);
    void setEnabledParamsBox(bool state);
    void setEnabledTab(bool state);
};

#endif // UICONTROLLER_H
