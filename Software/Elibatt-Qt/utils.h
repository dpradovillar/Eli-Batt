#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include <QValidator>

class UiValidator {
public:
    static QValidator* newHexValidator(QObject *parent = NULL);
    static QValidator* newDecValidator(QObject *parent = NULL);
    static QValidator* newAsciiValidator(QObject *parent = NULL);

    static QString sanitizeHex(const QString &hex);
    static QString sanitizeDec(const QString &dec);
    static QString sanitizeAscii(const QString &ascii);

    static qulonglong hex2dec(const QString &s);
    static qulonglong ascii2dec(const QString &s);
    static QString dec2hex(qulonglong x);
    static QString dec2ascii(qlonglong x);

    static QStringList replaceValue(const QStringList &haystack, const QString &needle, const QString &newValue);

};

#endif // UTILS_H
