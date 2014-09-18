#ifndef IDDIALOG_H
#define IDDIALOG_H

#include <QtCore>
#include <QDebug>
#include "utils.h"
#include "ui_iddialog.h"

class IdDialog : public QDialog {
    Q_OBJECT

    Ui::IdDialog *m_ui;
    qulonglong m_chosen_id;
    QValidator *m_hexValidator;
    QValidator *m_decValidator;
    QValidator *m_asciiValidator;

    bool m_enabledSignals;

public:
    explicit IdDialog();
    virtual ~IdDialog();

    int exec();

    void setId(qulonglong id2show);
    qulonglong getId();

private slots:
    void handleTextChangedLineEdit(const QString &s);
    void handleEditingFinishedLineEdit();

};
#endif // IDDIALOG_H
