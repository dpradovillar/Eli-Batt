#include "utils.h"

QValidator* UiValidator::newHexValidator(QObject *parent) {
    return new QRegExpValidator(QRegExp("0?[xX]?[0-9a-fA-F]{1,8}"), parent);
}

QValidator* UiValidator::newDecValidator(QObject *parent) {
    return new QRegExpValidator(QRegExp("[0-9]{1,10}"), parent);
}

QValidator* UiValidator::newAsciiValidator(QObject *parent) {
    return new QRegExpValidator(QRegExp("(.*){1,4}"), parent);
}

QString UiValidator::sanitizeHex(const QString &hex) {
    QString _hex = hex;
    _hex.remove("0x", Qt::CaseInsensitive);
    int toPad = qMax(0, 8 - _hex.length());
    return "0x" + QString(toPad, '0') + _hex.toUpper();
}

QString UiValidator::sanitizeDec(const QString &dec) {
    qulonglong val = dec.toULongLong();
    if (val > 4294967295LL) {
        val = 4294967295LL;
    }
    QString toRet = QString::number(val);
    int toPad = qMax(0, 10-toRet.length());
    return QString(toPad, '0') + toRet;
}

QString UiValidator::sanitizeAscii(const QString &ascii) {
    int toPad = qMax(0, 4-ascii.length());
    return QString(QString(toPad, '0') + ascii).right(4).toUpper();
}

#define DEC_VALUE(c)  ('0'<=c && c<='9' ? c-'0' : c-'A'+10)
#define HEX_VALUE(d)  (d < 10 ? d+'0' : d-10+'A')
#define CHAR_VALUE(d) (d < 32 ? '?' : (char)d)

qulonglong UiValidator::hex2dec(const QString &s) {
    QString val = s.toUpper();
    val.remove("0X");
    qulonglong res = 0;
    qulonglong pot = 1;
    for (int i=val.length()-1; i>=0; i--) {
        char c = val[i].toLatin1();
        res += pot*DEC_VALUE(c);
        pot *= 16;
    }
    return res;
}

qulonglong UiValidator::ascii2dec(const QString &s) {
    qulonglong res = 0;
    qulonglong pot = 1;
    for (int i=s.length()-1; i>=0; i--) {
        res += ((int)(s[i].toLatin1()))*pot;
        pot *= 256;
    }
    return res;
}

QString UiValidator::dec2hex(qulonglong x) {
    QString res(8, '0');
    for(int i=7; i>=0; i--) {
        int d = x % 16;
        res[i] = HEX_VALUE(d);
        x /= 16;
    }
    return "0x" + res.toUpper();
}

QString UiValidator::dec2ascii(qlonglong x) {
    QString res(4, '0');
    for(int i=3; i>=0; i--) {
        int d = x % 256;
        res[i] = CHAR_VALUE(d);
        x /= 256;
    }
    return res.toUpper();
}

QStringList UiValidator::replaceValue(const QStringList &haystack, const QString &needle, const QString &newValue) {
    QStringList res;
    foreach (const QString &s, haystack) {
        // If matches, replace with new value!
        if (s.compare(needle, Qt::CaseInsensitive) == 0) {
            res.append(newValue);
        }
        // Else, keep it in the list
        else {
            res.append(s);
        }
    }
    res.sort();
    return res;
}

