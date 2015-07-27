#include "reportgenerator.h"

#include <QPainter>
#include <QApplication>
#include <QTextDocument>


ReportGenerator::ReportGenerator(const QString &inputFilename, const QString &outputFilename) {
    m_inputFilename = inputFilename;
    m_outputFilename = outputFilename;
}

QString ReportGenerator::create() {
    QPdfWriter w(m_outputFilename);
    w.setPageSize(QPdfWriter::Letter);
    w.setPageMargins(QMarginsF(0, 0, 0, 0));
    w.setResolution(300);

    QPainter p(&w);
    m_pdfWriter = &w;
    m_printer = &p;

    // Header
    _setBrush(0x231F20);
    _fillRect(0, 10, 215.9, 30);
    _drawPixmap(170, 10, 30, 30, "res/logo_64.png");

    _setPen(0xFFFFFF);
    p.setFont(QFont("Helvetica", 36));
    _drawText(25, 30, "Reporte E-LiBatt");

    // Body
    _setPen(0x000000);
    p.setFont(QFont("Helvetica", 12));
    QVector<QStringList> d = QVector<QStringList>()
        << (QStringList() << "Período"                << ":" << "dd/mm/yy a dd/mm/yy")
        << (QStringList() << "Número de muestras"     << ":" << "86400")
        << (QStringList() << "Variables monitoreadas" << ":" << "T (°C), C (A), V (V)")
    ;

    _drawTable(25, 50,
        QList<double>() << 50 << 6 << 80,
        8,
        d
    );

    p.end();

    return "";
}

int ReportGenerator::convertX(qreal x_mm) {
    const qreal wPage = 2550.0;
    return (int)(x_mm * wPage / 215.9);
}
int ReportGenerator::convertY(qreal y_mm) {
    const qreal hPage = 3300.0;
    return (int)(y_mm * hPage / 279.4);
}

quint32 ReportGenerator::convertW(qreal width_mm) {
    return convertX(qAbs(width_mm));
}
quint32 ReportGenerator::convertH(qreal height_mm) {
    return convertY(qAbs(height_mm));
}

QPoint ReportGenerator::convert(qreal x_mm, qreal y_mm) {
    return QPoint(convertX(x_mm), convertY(y_mm));
}

void ReportGenerator::_setPen(int hex) {
    m_printer->setPen(QColor(QRgb(hex)));
}
void ReportGenerator::_setBrush(int hex) {
    m_printer->setBrush(QColor(QRgb(hex)));
}
void ReportGenerator::_fillRect(double x_mm, double y_mm, double w_mm, double h_mm) {
    m_printer->fillRect(convertX(x_mm), convertY(y_mm), convertW(w_mm), convertH(h_mm), m_printer->brush());
}
void ReportGenerator::_drawPixmap(double x_mm, double y_mm, double w_mm, double h_mm, const QString &resourceName) {
    m_printer->drawPixmap(convertX(x_mm), convertY(y_mm), convertW(w_mm), convertH(h_mm), QPixmap(resourceName));
}
void ReportGenerator::_drawText(double x_mm, double y_mm, const QString &text) {
    m_printer->drawText(convertX(x_mm), convertY(y_mm), text);
}
void ReportGenerator::_drawTable(double x_mm, double y_mm, const QList<double> &colWidths, double cellHeight, const QVector<QStringList> &rows) {
    int numRows = rows.size();
    int numCols = (numRows > 0 ? rows[0].size() : 0);

    if (numCols != rows.size()) {
        qDebug() << "Number of columns width should be the same as the number of columns!";
        return;
    }

    for (int i = 0; i < numRows; i++) {
        if (rows[i].size() != numCols) {
            qDebug() << "All columns should be the same length!";
            return;
        }
    }
    double xGap = 0.0;
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {
            _drawText(x_mm+xGap, y_mm+j*cellHeight, rows[j][i]);
        }
        xGap += colWidths[i];
    }
}
