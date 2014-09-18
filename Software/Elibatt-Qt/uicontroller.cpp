#include "uicontroller.h"

UiController::UiController()
    : m_ui(NULL)
{
}

UiController::~UiController() {
}

void UiController::start() {
    m_ui->baud->setCurrentIndex(3); // 9600bps

    m_connectionStatus.setConnectButton(m_ui->connect_button);
    m_connectionStatus.setUi(m_ui);

    connect(&m_serialMonitor, SIGNAL(onSerialListChanged(QStringList, bool)), this, SLOT(handleSerialListChanged(const QStringList&, bool)));
    connect(&m_connectionStatus, SIGNAL(onlineTimerSignal(quint64)), this, SLOT(handleOnlineTimerTimeout(quint64)));
    connect(m_ui->connect_button, SIGNAL(clicked()), this, SLOT(handleConnectButtonClicked()));

    m_eepromController.setUi(m_ui);
    m_eepromController.start();

    m_gpsController.setUi(m_ui);

    m_serialMonitor.setComboBox(m_ui->port);
    m_serialMonitor.start();

    m_serialThread = NULL;

    // Auto connect to first COM port after 2seconds of started! Remove this line!!
    //QTimer::singleShot(600, this, SLOT(handleConnectButtonClicked()));
}

void UiController::stop() {
    m_serialMonitor.stop();
    m_eepromController.stop();
}

void UiController::setUi(Ui::UiDialog *ui) {
    m_ui = ui;
}

void UiController::handleSerialListChanged(const QStringList &list, bool forceDisconnect) {

    if (forceDisconnect) {
        setEnabledParams(true);
        m_ui->operation_status->clear();
    }

    if (list.isEmpty() || forceDisconnect) {
        if (m_connectionStatus.isConnected()) {
            m_connectionStatus.disconnectCOM();
        }
        setEnabledConnect(!list.isEmpty());
    } else {
        if (!m_connectionStatus.isConnected()) {
            setEnabledConnect(true);
        }
    }
}

void UiController::handleOnlineTimerTimeout(quint64 diff) {
    m_ui->operation_status->setText(QString("%1 secs").arg(diff));
}

void UiController::handleConnectButtonClicked() {
    if (m_connectionStatus.isConnected()) {
        setEnabledParams(true);
        m_ui->operation_status->clear();
        m_connectionStatus.disconnectCOM();
    } else {
        m_connectionStatus.connectCOM(m_ui->port->currentText(), m_ui->baud->currentText().toInt());
        setEnabledParams(false);
    }
}

void UiController::setEnabledParams(bool state) {
    m_ui->port->setEnabled(state);
    m_ui->baud->setEnabled(state);
}

void UiController::setEnabledConnect(bool state) {
    m_ui->connect_button->setEnabled(state);
    m_ui->baud->setEnabled(state);
}

void UiController::setEnabledParamsBox(bool state) {
    m_ui->port->setEnabled(state);
    m_ui->baud->setEnabled(state);
    m_ui->connect_button->setEnabled(state);
}

void UiController::setEnabledTab(bool state) {
    m_ui->tabs->setEnabled(state);
}
