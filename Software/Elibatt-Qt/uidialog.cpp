#include "uidialog.h"

UiDialog::UiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UiDialog)
{
    ui->setupUi(this);

    m_uiController.setUi(ui);
    m_uiController.start();
}

UiDialog::~UiDialog()
{
    m_uiController.stop();
    delete ui;
}

