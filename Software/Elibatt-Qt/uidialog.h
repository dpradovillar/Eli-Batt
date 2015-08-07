#ifndef UIDIALOG_H
#define UIDIALOG_H

#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QList>
#include <QTimer>
#include <QSerialPortInfo>

#include "ui_uidialog.h"
#include "uicontroller.h"

class UiDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::UiDialog *ui;
    UiController m_uiController;

public:
    explicit UiDialog(QWidget *parent = 0);
    ~UiDialog();
};

#endif // UIDIALOG_H
