#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <QtCore>
#include <QPrinter>
#include <QPdfWriter>

class ReportGenerator {
private:
    QString m_inputFilename;
    QString m_outputFilename;

    QPdfWriter *m_pdfWriter;
    QPainter *m_printer;

public:
    ReportGenerator(const QString &inputFilename, const QString &outputFilename);

    QString create();

    int convertX(qreal x_mm);
    int convertY(qreal y_mm);
    quint32 convertW(qreal width_mm);
    quint32 convertH(qreal height_mm);

    QPoint convert(qreal x_mm, qreal y_mm);

    void _setPen(int hex);
    void _setBrush(int hex);
    void _fillRect(double x_mm, double y_mm, double w_mm, double h_mm);
    void _drawPixmap(double x_mm, double y_mm, double w_mm, double h_mm, const QString &resourceName);
    void _drawText(double x_mm, double y_mm, const QString &text);
    void _drawTable(double x_mm, double y_mm, const QList<double> &colWidths, double cellHeight, const QVector<QStringList> &rows);
};

#endif // REPORT_GENERATOR_H
