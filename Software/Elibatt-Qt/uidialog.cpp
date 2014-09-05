#include "uidialog.h"

#include <QDateTime>

#define FULL_FORMAT  "yyyy-MM-dd hh:mm:ss"

UiDialog::UiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UiDialog)
{
    ui->setupUi(this);
    aTimer.setInterval(300);

    connect(ui->writeButton, SIGNAL(clicked()), this, SLOT(sendDatetime()));
    connect(&aTimer, SIGNAL(timeout()), this, SLOT(processTimeout()));

    aTimer.start();
}

UiDialog::~UiDialog()
{
    aTimer.stop();
    delete ui;
}

void UiDialog::processTimeout() {
    QDateTime dt = QDateTime::currentDateTime();
    ui->datetimeLabel->setText(dt.toString(FULL_FORMAT));
}

void UiDialog::sendDatetime() {
    // wua wua uwaa

    QDateTime dt = QDateTime::currentDateTime();
    ui->slaveOutputLabel->setText("RTC written succesfully!");

    QTimer::singleShot(2500, this, SLOT(cleanUi()));
}

void UiDialog::cleanUi() {
    ui->slaveOutputLabel->clear();
}

