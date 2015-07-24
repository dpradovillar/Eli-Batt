#include "reportgenerator.h"

#include <QPainter>
#include <QApplication>
#include <QTextDocument>
#include <QPrinter>

ReportGenerator::ReportGenerator(const QString &inputFilename, const QString &outputFilename) {
    m_inputFilename = inputFilename;
    m_outputFilename = outputFilename;
}

QString ReportGenerator::create() {
    // Create temporary HTML file with the report.
    QString htmlContent = QString(""
        "<html>"
        "<head>"
        "<title>Document 1.0</title>"
        "</head>"
        "<body>"
        "<p>%1</p>"
        "</body>"
        "</html>"
    ).arg(1);

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(m_outputFilename);

    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    return "";
}
