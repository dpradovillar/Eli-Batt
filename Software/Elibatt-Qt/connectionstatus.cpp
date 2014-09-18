#include "connectionstatus.h"

ConnectionStatus::ConnectionStatus() :
    m_connected(false)
{
    m_onlineTimer.setInterval(300);
    QObject::connect(&m_onlineTimer, SIGNAL(timeout()), this, SLOT(onlineTimerTimeout()));
}

ConnectionStatus::~ConnectionStatus() {
}

void ConnectionStatus::setConnectButton(QPushButton *connectButton) {
    m_connectButton = connectButton;
}

void ConnectionStatus::setUi(Ui::UiDialog *ui) {
    m_ui = ui;

    connect(m_ui->foo_button, SIGNAL(clicked()), this, SLOT(handleFooClick()));
}

bool ConnectionStatus::isConnected() {
    return m_connected;
}

void ConnectionStatus::connectCOM(const QString &port, quint32 bauds) {
    m_connected = true;

    m_onlineTimerStart = QDateTime::currentMSecsSinceEpoch();

    qDebug() << "Connecting to:" << port << ", at:" << bauds << "bps";

    m_serialThread = new SerialThread;

    connect(m_serialThread, SIGNAL(newResponse(Message)),
            this, SLOT(handleResponse(Message)));
    connect(m_serialThread, SIGNAL(newError(QString)),
            this, SLOT(handleError(QString)));
    connect(m_serialThread, SIGNAL(newTimeout(QString)),
            this, SLOT(handleTimeout(QString)));

    m_serialThread->setParams(port, bauds);
    m_serialThread->start();

    m_onlineTimer.start();

    m_connectButton->setText("Disconnect");
    m_ui->tabs->setEnabled(true);
}

void ConnectionStatus::disconnectCOM() {
    m_connected = false;
    m_onlineTimer.stop();

    qDebug() << "ConnectionStatus::disconnectCOM() : Disconnecting";

    if (m_serialThread) {
        m_serialThread->stop();
        m_serialThread->wait();
        delete m_serialThread;
        m_serialThread = NULL;
    }

    qDebug() << "ConnectionStatus::disconnectCOM() : Disconnected";

    m_connectButton->setText("Connect");
    m_ui->tabs->setEnabled(false);
}

void ConnectionStatus::onlineTimerTimeout() {
    qint64 diff = QDateTime::currentMSecsSinceEpoch() - m_onlineTimerStart;
    emit onlineTimerSignal(diff/1000);
}

void ConnectionStatus::handleFooClick() {
    qDebug() << "                                                  Sending stuff";

    Message m;
    m.m_crc = 123;

    m.m_data[0] = 10;
    m.m_data[1] = 11;
    m.m_data[2] = 12;
    m.m_data[3] = 13;

    m.m_fromId[0] = 20;
    m.m_fromId[1] = 21;
    m.m_fromId[2] = 22;
    m.m_fromId[3] = 23;

    m.m_targetId[0] = 30;
    m.m_targetId[1] = 31;
    m.m_targetId[2] = 32;
    m.m_targetId[3] = 33;

    m.m_status = 105;

    m.m_type = 106;

    m_serialThread->transmitPackage(m);
}

void ConnectionStatus::handleError(const QString &err) {
    qDebug() << "ConnectionStatus::handleError :" << err;
}

void ConnectionStatus::handleTimeout(const QString &err) {
    qDebug() << "ConnectionStatus::handleTimeout :" << err;
}

void ConnectionStatus::handleResponse(const Message &msg) {
    qDebug() << "ConnectionStatus::handleResponse :" << msg.toString();
}
