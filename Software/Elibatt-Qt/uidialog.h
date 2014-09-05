#ifndef UIDIALOG_H
#define UIDIALOG_H

#include <QDialog>
#include <QTimer>

#include "ui_uidialog.h"

class UiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UiDialog(QWidget *parent = 0);
    ~UiDialog();

private:
    Ui::UiDialog *ui;
    QTimer aTimer;

private slots:
    void processTimeout();
    void cleanUi();
    void sendDatetime();
};

#endif // UIDIALOG_H
