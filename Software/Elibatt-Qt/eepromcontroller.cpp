#include "eepromcontroller.h"

void EepromController::updateControls() {
    int size = m_listModel.rowCount();
    m_ui->remove_all_slave_button->setEnabled(size > 0);
    m_ui->add_slave_button->setEnabled(size < MAX_BOARDS);
    m_ui->edit_slave_button->setEnabled(m_ui->registered_ids->currentIndex().isValid());
    m_ui->remove_slave_button->setEnabled(m_ui->registered_ids->currentIndex().isValid());

}

EepromController::EepromController()
{
    m_hexValidator = UiValidator::newHexValidator();
    m_decValidator = UiValidator::newDecValidator();
    m_asciiValidator = UiValidator::newAsciiValidator();
}

EepromController::~EepromController()
{
    delete m_hexValidator;
    delete m_decValidator;
    delete m_asciiValidator;
}

void EepromController::setUi(Ui::UiDialog *uiDialog){
   m_ui = uiDialog;

   // Read ID controls
   m_ui->read_hex_lineedit->setValidator(m_hexValidator);
   m_ui->read_dec_lineedit->setValidator(m_decValidator);
   m_ui->read_ascii_lineedit->setValidator(m_asciiValidator);

   // Read ID controls
   m_ui->write_hex_lineedit->setValidator(m_hexValidator);
   m_ui->write_dec_lineedit->setValidator(m_decValidator);
   m_ui->write_ascii_lineedit->setValidator(m_asciiValidator);

   connect(m_ui->write_hex_lineedit, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));
   connect(m_ui->write_dec_lineedit, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));
   connect(m_ui->write_ascii_lineedit, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));

   connect(m_ui->write_hex_lineedit, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));
   connect(m_ui->write_dec_lineedit, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));
   connect(m_ui->write_ascii_lineedit, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));

   connect(m_ui->read_id_button, SIGNAL(clicked()), this, SLOT(handleReadIdClick()));
   connect(m_ui->write_id_button, SIGNAL(clicked()), this, SLOT(handleWriteIdClick()));

   connect(m_ui->read_all_button, SIGNAL(clicked()), this, SLOT(handleReadAllClick()));
   connect(m_ui->add_slave_button, SIGNAL(clicked()), this, SLOT(handleAddSlaveClick()));
   connect(m_ui->edit_slave_button, SIGNAL(clicked()), this, SLOT(handleEditSlaveClick()));
   connect(m_ui->remove_slave_button, SIGNAL(clicked()), this, SLOT(handleRemoveSlaveClick()));
   connect(m_ui->remove_all_slave_button, SIGNAL(clicked()), this, SLOT(handleRemoveAllSlaveClick()));

   connect(m_ui->registered_ids, SIGNAL(clicked(QModelIndex)), this, SLOT(handleRegisteredIdsClicked(const QModelIndex &)));

   m_ui->registered_ids->setModel(&m_listModel);
}

void EepromController::start() {
    m_enabledSignals = true;
}

void EepromController::stop() {
}

void EepromController::setReadId(qulonglong dec) {
    QString hexVal = UiValidator::dec2hex(dec);
    QString decVal = UiValidator::sanitizeDec(QString::number(dec));
    QString asciiVal = UiValidator::dec2ascii(dec);

    m_ui->read_hex_lineedit->setText(hexVal);
    m_ui->read_dec_lineedit->setText(decVal);
    m_ui->read_ascii_lineedit->setText(asciiVal);
}

void EepromController::handleReadIdClick() {
    m_ui->read_hex_lineedit->clear();
    m_ui->read_dec_lineedit->clear();
    m_ui->read_ascii_lineedit->clear();

    qulonglong aux = m_ui->write_dec_lineedit->text().toULongLong();
    setReadId(aux);

    qDebug() << "Reading ID from COM port!";
}

void EepromController::handleWriteIdClick() {
    qDebug() << "Writing ID from COM port!";
}

void EepromController::handleReadAllClick() {
    qDebug() << "Reading all the IDs";
}

void EepromController::handleAddSlaveClick() {
    IdDialog idDiag;
    idDiag.setWindowTitle("Add Slave ID");
    idDiag.setId(0);

    while(true) {
        if (idDiag.exec() != QDialog::Accepted) {
            break;
        }

        qulonglong id = idDiag.getId();

        QStringList oldValues = m_listModel.stringList();
        QString newValue = UiValidator::dec2hex(id);

        if (oldValues.contains(newValue)){
            QString msg = QString("The ID %1 is already on the list.\n Please, choose another.").arg(newValue);
            QMessageBox::warning(&idDiag, "Error", msg);
        } else {
            oldValues << newValue;
            oldValues.sort();
            m_listModel.setStringList(oldValues);
            updateControls();
            break;
        }
    }
}

void EepromController::handleEditSlaveClick() {
    qDebug() << "Editing existing ID";

    QModelIndex current = m_ui->registered_ids->currentIndex();

    if (current.isValid()) {
        QString toEdit = m_listModel.stringList().at(current.row());

        IdDialog idDiag;
        idDiag.setWindowTitle("Edit Slave ID");
        idDiag.setId(UiValidator::hex2dec(toEdit));

        while(true) {
            if (idDiag.exec() != QDialog::Accepted) {
                break;
            }

            qulonglong id = idDiag.getId();

            QStringList oldValues = m_listModel.stringList();
            QString newValue = UiValidator::dec2hex(id);

            // It's ok to edit a number and either cancel or accept it without changing it.
            if (oldValues.contains(newValue) &&
                    newValue.compare(toEdit, Qt::CaseInsensitive) != 0){
                QString msg = QString("The ID %1 is already on the list.\n Please, choose another.").arg(newValue);
                QMessageBox::warning(&idDiag, "Error", msg);
            } else {
                m_listModel.setStringList(UiValidator::replaceValue(oldValues, toEdit, newValue));
                updateControls();
                break;
            }
        }
    }
}

void EepromController::handleRemoveSlaveClick() {
    QModelIndex current = m_ui->registered_ids->currentIndex();
    if (current.isValid()) {
        qDebug() << "Removing:" << m_listModel.stringList().at(current.row());
        m_listModel.removeRow(current.row());
    }

    updateControls();
}

void EepromController::handleRemoveAllSlaveClick() {
    qDebug() << "Removing all the IDs";

    m_listModel.setStringList(QStringList());

    updateControls();
}

void EepromController::handleRegisteredIdsClicked(const QModelIndex &m) {
    Q_UNUSED(m);
    updateControls();
}

void EepromController::handleTextChangedLineEdit(const QString &s) {

    if (m_enabledSignals) {
        QObject *source = sender();
        QString hexVal, decVal, asciiVal;
        qulonglong aux; // For intermediate decimal conversions

        if (source == m_ui->write_hex_lineedit) {
            hexVal = UiValidator::sanitizeHex(s);
            aux = UiValidator::hex2dec(hexVal);

            decVal = UiValidator::sanitizeDec(QString::number(aux));
            asciiVal = UiValidator::dec2ascii(aux);

            m_enabledSignals = false;
            m_ui->write_dec_lineedit->setText(decVal);
            m_ui->write_ascii_lineedit->setText(asciiVal);
            m_enabledSignals = true;

        } else if (source == m_ui->write_dec_lineedit) {
            decVal = UiValidator::sanitizeDec(s);
            aux = s.toULongLong();

            hexVal = UiValidator::dec2hex(aux);
            asciiVal = UiValidator::dec2ascii(aux);

            m_enabledSignals = false;
            m_ui->write_hex_lineedit->setText(hexVal);
            m_ui->write_ascii_lineedit->setText(asciiVal);
            m_enabledSignals = true;

        } else if (source == m_ui->write_ascii_lineedit) {
            asciiVal = UiValidator::sanitizeAscii(s);
            aux = UiValidator::ascii2dec(asciiVal);

            hexVal = UiValidator::dec2hex(aux);
            decVal = UiValidator::sanitizeDec(QString::number(UiValidator::ascii2dec(asciiVal)));

            m_enabledSignals = false;
            m_ui->write_hex_lineedit->setText(hexVal);
            m_ui->write_dec_lineedit->setText(decVal);
            m_enabledSignals = true;
        }
    }
}

void EepromController::handleEditingFinishedLineEdit() {
    if (m_enabledSignals) {
        QObject *source = sender();
        if (source == m_ui->write_hex_lineedit) {
            m_enabledSignals = false;
            m_ui->write_hex_lineedit->setText(UiValidator::sanitizeHex(m_ui->write_hex_lineedit->text()));
            m_enabledSignals = true;
        } else if (source == m_ui->write_dec_lineedit) {
            m_enabledSignals = false;
            QString sanitized = UiValidator::sanitizeDec(m_ui->write_dec_lineedit->text());

            // Dec values have to be sanitized and converted again because of the MAX_INT
            // restriction not being consequent with the input character restriction
            m_ui->write_dec_lineedit->setText(sanitized);
            qulonglong aux = sanitized.toULongLong();
            m_ui->write_hex_lineedit->setText(UiValidator::dec2hex(aux));
            m_ui->write_ascii_lineedit->setText(UiValidator::dec2ascii(aux));

            m_enabledSignals = true;
        } else if (source == m_ui->write_ascii_lineedit) {
            m_enabledSignals = false;
            m_ui->write_ascii_lineedit->setText(UiValidator::sanitizeAscii(m_ui->write_ascii_lineedit->text()));
            m_enabledSignals = true;
        }
    }
}
