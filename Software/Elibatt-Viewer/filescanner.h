#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <QtCore>
#include <QPrinter>
#include <QPdfWriter>

class FileScanner {
private:
    QString m_inputFilename;

public:
    qint64 m_count;

    QDateTime m_minDateTime;
    qreal m_minTemperature, m_minCurrent, m_minVoltage;

    QDateTime m_maxDateTime;
    qreal m_maxTemperature, m_maxCurrent, m_maxVoltage;

    FileScanner(const QString &inputFilename);

    QString scan();

private:
    bool m_first;

    void processLine(const QString &line);
};

#endif // FILE_SCANNER_H
