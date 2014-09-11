#include "uidialog.h"

#include <QSerialPortInfo>
#include <QDateTime>
#include <QList>
#include <QDebug>

#define FULL_FORMAT  "yyyy-MM-dd hh:mm:ss"

UiDialog::UiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UiDialog)
{
    ui->setupUi(this);
    aTimer.setInterval(300);

    ui->baud->setCurrentIndex(5);

    //connect(ui->writeButton, SIGNAL(clicked()), this, SLOT(sendDatetime()));
    connect(&aTimer, SIGNAL(timeout()), this, SLOT(checkSerialPorts()));

    aTimer.start();
}

UiDialog::~UiDialog()
{
    aTimer.stop();
    delete ui;
}

void UiDialog::lockUi() {
    ui->board_type->setEnabled(false);
    ui->port->setEnabled(false);
    ui->baud->setEnabled(false);
    ui->connect_button->setEnabled(false);
    ui->tabs->setEnabled(false);
}

void UiDialog::unlockUi() {
    ui->board_type->setEnabled(true);
    ui->port->setEnabled(true);
    ui->baud->setEnabled(true);
    ui->connect_button->setEnabled(true);
    ui->tabs->setEnabled(true);
}

void UiDialog::processTimeout() {
    QDateTime dt = QDateTime::currentDateTime();
    ui->datetimeLabel->setText(dt.toString(FULL_FORMAT));
}

void UiDialog::sendDatetime() {
    QDateTime dt = QDateTime::currentDateTime();
   // ui->slaveOutputLabel->setText("RTC written succesfully!");

    QTimer::singleShot(2500, this, SLOT(cleanUi()));
}

void UiDialog::cleanUi() {
   // ui->slaveOutputLabel->clear();
}

void UiDialog::checkSerialPorts() {
    QList<QSerialPortInfo> availableSerials = QSerialPortInfo::availablePorts();
    ui->port->clear();
    //qDebug() << "# of available serial ports:" << availableSerials.size() << endl;
    foreach (const QSerialPortInfo &info, availableSerials ) {
        ui->port->addItem(info.portName());
    }

    if (availableSerials.empty()) {
        lockUi();
    } else {
        unlockUi();
    }
}
