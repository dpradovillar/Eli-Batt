#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <QtCore>

class ReportGenerator {
private:
    QString m_inputFilename;
    QString m_outputFilename;

public:
    ReportGenerator(const QString &inputFilename, const QString &outputFilename);

    QString create();
};

#endif // REPORT_GENERATOR_H
