#include "filescanner.h"

#include <QPainter>
#include <QApplication>
#include <QTextDocument>

FileScanner::FileScanner(const QString &inputFilename) {
    m_inputFilename = inputFilename;
}

QString FileScanner::scan() {
    QFile file(m_inputFilename);
    if (!file.open(QIODevice::ReadOnly)) {
        return "Cannot open file " + m_inputFilename;
    }
    QTextStream in(&file);
    m_first = true;
    QString temp;
    m_count = 0;
    while(!(temp = in.readLine()).isNull()) {
        m_count++;
        processLine(temp);
    }
    file.close();
    return "";
}

void FileScanner::processLine(const QString &line) {
    if (line.isEmpty()) {
        return;
    }
    QStringList parts = line.split(';');
    if (m_first) {
        m_minDateTime = m_maxDateTime = QDateTime::fromString(parts[0], "yyyyMMdd'T'hhmmss");
        m_minTemperature = m_maxTemperature = parts[1].toDouble();
        m_minCurrent = m_maxCurrent = parts[2].toDouble();
        m_minVoltage = m_maxVoltage = parts[3].toDouble();
        m_first = false;
    } else {
        QDateTime dt = QDateTime::fromString(parts[0], "yyyyMMdd'T'hhmmss");
        qreal t = parts[1].toDouble();
        qreal c = parts[2].toDouble();
        qreal v = parts[3].toDouble();
        m_minDateTime = qMin(m_minDateTime, dt);
        m_maxDateTime = qMax(m_maxDateTime, dt);
        m_minTemperature = qMin(m_minTemperature, t);
        m_maxTemperature = qMax(m_maxTemperature, t);
        m_minCurrent = qMin(m_minCurrent, c);
        m_maxCurrent = qMax(m_maxCurrent, c);
        m_minVoltage = qMin(m_minVoltage, v);
        m_maxVoltage = qMax(m_maxVoltage, v);
    }
}
