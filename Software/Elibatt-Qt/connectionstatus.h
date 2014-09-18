#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

#include <QTabWidget>
#include <QPushButton>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QtCore>

#include "serialthread.h"

#include "ui_uidialog.h"

class ConnectionCallback;

class ConnectionStatus : public QObject
{
    Q_OBJECT

private:
    bool m_connected;
    QTimer m_onlineTimer;
    quint64 m_onlineTimerStart;

    QPushButton *m_connectButton;
    Ui::UiDialog *m_ui;

    SerialThread *m_serialThread;

public:
    explicit ConnectionStatus();
    ~ConnectionStatus();

    void setConnectButton(QPushButton *connectButton);
    void setUi(Ui::UiDialog *ui);

    bool isConnected();

    void connectCOM(const QString &port, quint32 bauds);
    void disconnectCOM();

signals:
    void onlineTimerSignal(quint64 elapsedTime);

private slots:
    void onlineTimerTimeout();

    void handleFooClick();

    void handleError(const QString &err);
    void handleTimeout(const QString &err);
    void handleResponse(const Message &msg);
};


#endif // CONNECTIONSTATUS_H
