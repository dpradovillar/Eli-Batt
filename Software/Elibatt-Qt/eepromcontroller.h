#ifndef EEPROMCONTROLLER_H
#define EEPROMCONTROLLER_H

#include <QDebug>
#include <QMessageBox>
#include <QStringListModel>
#include <QtCore>
#include <QModelIndex>

#include "utils.h"
#include "ui_uidialog.h"
#include "iddialog.h"

#define MAX_BOARDS 12

class EepromController : public QObject
{
    Q_OBJECT

    Ui::UiDialog *m_ui;

    QValidator *m_hexValidator;
    QValidator *m_decValidator;
    QValidator *m_asciiValidator;

    QStringListModel m_listModel;

    bool m_enabledSignals;

    void updateControls();

public:
    explicit EepromController();
    ~EepromController();

    void setUi(Ui::UiDialog *uiDialog);

    void start();
    void stop();

    void setReadId(qulonglong dec);

private slots:
    void handleReadIdClick();
    void handleWriteIdClick();

    void handleReadAllClick();
    void handleAddSlaveClick();
    void handleEditSlaveClick();
    void handleRemoveSlaveClick();
    void handleRemoveAllSlaveClick();

    void handleRegisteredIdsClicked(const QModelIndex &);

    void handleTextChangedLineEdit(const QString &);
    void handleEditingFinishedLineEdit();
};



#endif // EEPROMCONTROLLER_H
