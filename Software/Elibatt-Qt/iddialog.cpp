#include "iddialog.h"

IdDialog::IdDialog() :
    m_ui(new Ui::IdDialog)
{
    m_ui->setupUi(this);

    m_hexValidator = UiValidator::newHexValidator();
    m_decValidator = UiValidator::newDecValidator();
    m_asciiValidator = UiValidator::newAsciiValidator();

    m_ui->hex->setValidator(m_hexValidator);
    m_ui->dec->setValidator(m_decValidator);
    m_ui->ascii->setValidator(m_asciiValidator);

    connect(m_ui->hex, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));
    connect(m_ui->dec, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));
    connect(m_ui->ascii, SIGNAL(textChanged(QString)), this, SLOT(handleTextChangedLineEdit(const QString &)));

    connect(m_ui->hex, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));
    connect(m_ui->dec, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));
    connect(m_ui->ascii, SIGNAL(editingFinished()), this, SLOT(handleEditingFinishedLineEdit()));
}

IdDialog::~IdDialog() {
    delete m_ui;

    delete m_hexValidator;
    delete m_decValidator;
    delete m_asciiValidator;
}

int IdDialog::exec() {
    // Initial setup of the values of the modal dialog
    m_enabledSignals = false;
    QString sanitized = UiValidator::sanitizeDec(QString::number(m_chosen_id));
    m_ui->dec->setText(sanitized);
    m_ui->hex->setText(UiValidator::dec2hex(m_chosen_id));
    m_ui->ascii->setText(UiValidator::dec2ascii(m_chosen_id));
    m_enabledSignals = true;

    return QDialog::exec();
}

void IdDialog::setId(qulonglong id2show) {
    m_chosen_id = id2show;
}

qulonglong IdDialog::getId() {
    return m_chosen_id;
}

void IdDialog::handleTextChangedLineEdit(const QString &s) {

    if (m_enabledSignals) {
        QObject *source = sender();
        QString hexVal, decVal, asciiVal;
        qulonglong aux; // For intermediate decimal conversions

        if (source == m_ui->hex) {
            hexVal = UiValidator::sanitizeHex(s);
            aux = UiValidator::hex2dec(hexVal);

            decVal = UiValidator::sanitizeDec(QString::number(aux));
            asciiVal = UiValidator::dec2ascii(aux);

            m_enabledSignals = false;
            m_ui->dec->setText(decVal);
            m_ui->ascii->setText(asciiVal);
            m_enabledSignals = true;

        } else if (source == m_ui->dec) {
            decVal = UiValidator::sanitizeDec(s);
            aux = s.toULongLong();

            hexVal = UiValidator::dec2hex(aux);
            asciiVal = UiValidator::dec2ascii(aux);

            m_enabledSignals = false;
            m_ui->hex->setText(hexVal);
            m_ui->ascii->setText(asciiVal);
            m_enabledSignals = true;

        } else if (source == m_ui->ascii) {
            asciiVal = UiValidator::sanitizeAscii(s);
            aux = UiValidator::ascii2dec(asciiVal);

            hexVal = UiValidator::dec2hex(aux);
            decVal = UiValidator::sanitizeDec(QString::number(UiValidator::ascii2dec(asciiVal)));

            m_enabledSignals = false;
            m_ui->hex->setText(hexVal);
            m_ui->dec->setText(decVal);
            m_enabledSignals = true;
        }
    }
}

void IdDialog::handleEditingFinishedLineEdit() {
    if (m_enabledSignals) {
        QObject *source = sender();
        if (source == m_ui->hex) {
            m_enabledSignals = false;
            m_ui->hex->setText(UiValidator::sanitizeHex(m_ui->hex->text()));
            m_enabledSignals = true;
        } else if (source == m_ui->dec) {
            m_enabledSignals = false;
            QString sanitized = UiValidator::sanitizeDec(m_ui->dec->text());

            // Dec values have to be sanitized and converted again because of the MAX_INT
            // restriction not being consequent with the input character restriction
            m_ui->dec->setText(sanitized);
            qulonglong aux = sanitized.toULongLong();
            m_ui->hex->setText(UiValidator::dec2hex(aux));
            m_ui->ascii->setText(UiValidator::dec2ascii(aux));

            m_enabledSignals = true;
        } else if (source == m_ui->ascii) {
            m_enabledSignals = false;
            m_ui->ascii->setText(UiValidator::sanitizeAscii(m_ui->ascii->text()));
            m_enabledSignals = true;
        }

        m_chosen_id = m_ui->dec->text().toULongLong();
    }
}
