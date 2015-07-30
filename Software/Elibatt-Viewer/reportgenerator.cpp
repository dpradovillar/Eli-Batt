#include "reportgenerator.h"

#include "filescanner.h"

#include <QPainter>
#include <QApplication>
#include <QTextDocument>

ReportGenerator::ReportGenerator(const QString &inputFilename, const QString &outputFilename) {
    m_inputFilename = inputFilename;
    m_outputFilename = outputFilename;
}

QString ReportGenerator::create() {

    FileScanner fs(m_inputFilename);
    QString status = fs.scan();
    if (!status.isEmpty()) {
        return status;
    }

    QPdfWriter w(m_outputFilename);
    w.setPageSize(QPdfWriter::Letter);
    w.setPageMargins(QMarginsF(0, 0, 0, 0));
    w.setResolution(300);

    QPainter p(&w);
    m_pdfWriter = &w;
    m_printer = &p;

    // Header
    _setBrush(0x231F20);
    QRectF hRect(0, 10, 215.9, 30);
    _fillRect(hRect);
    _drawPixmap(QRectF(170, 10, 30, 30), "res/logo_64.png");

    _setPen(0xFFFFFF);
    _setFont(36);
    _drawText(hRect, "Reporte E-LiBatt", Qt::AlignCenter);

    qreal plot_width = 165.9;
    qreal plot_height = 50.0;

    // Body
    _setPen(0x000000);
    _setFont(12);
    _drawTable(25, 45,
        QList<double>() << 50 << 6 << 80,
        5,
        QVector<QStringList>()
           << (QStringList() << "Período"                << ":" << QString("%1 a %2")
               .arg(fs.m_minDateTime.date().toString("yyyy-MM-dd"))
               .arg(fs.m_maxDateTime.date().toString("yyyy-MM-dd")))
           << (QStringList() << "Número de muestras"     << ":" << QString::number(fs.m_count))
           << (QStringList() << "Variables monitoreadas" << ":" << "T (°C), C (A), V (V) Medias")
    );

    _setPen(0x000000);

    qreal temperatureStep = 5.0;
    qreal currentStep = 2.0;
    qreal voltageStep = 3.0;

    qreal temperatureLowBound, temperatureHighBound;
    qreal currentLowBound, currentHighBound;
    qreal voltageLowBound, voltageHighBound;

    // Temperatura
    _setFont(14);
    _drawText(QRectF(25, 60, 165.9, 10), "Temperatura (C°)", Qt::AlignCenter);
    _setFont(10);
    _drawFrameGraph(QRectF(25, 70, plot_width, plot_height),
        splitSpan(
            temperatureLowBound = makeLowerBound(fs.m_minTemperature, temperatureStep),
            temperatureHighBound = makeUpperBound(fs.m_maxTemperature, temperatureStep),
            6
        ),
        splitSpan(fs.m_minDateTime, fs.m_maxDateTime, 6)
    );

    // Corriente
    _setFont(14);
    _drawText(QRectF(25, 130, 165.9, 10), "Corriente (A)", Qt::AlignCenter);
    _setFont(10);
    _drawFrameGraph(QRectF(25, 140, plot_width, plot_height),
        splitSpan(
            currentLowBound = makeLowerBound(fs.m_minCurrent, currentStep),
            currentHighBound = makeUpperBound(fs.m_maxCurrent, currentStep),
            6
        ),
        splitSpan(fs.m_minDateTime, fs.m_maxDateTime, 6)
    );

    // Corriente
    _setFont(14);
    _drawText(QRectF(25, 200, 165.9, 10), "Voltaje (V)", Qt::AlignCenter);
    _setFont(10);
    _drawFrameGraph(QRectF(25, 210, plot_width, plot_height),
        splitSpan(
            voltageLowBound = makeLowerBound(fs.m_minVoltage, voltageStep),
            voltageHighBound = makeUpperBound(fs.m_maxVoltage, voltageStep),
            6
        ),
        splitSpan(fs.m_minDateTime, fs.m_maxDateTime, 6)
    );

    // Draw the actual graph plots
    QFile inputFile(m_inputFilename);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        return "No se puede abrir " + m_inputFilename;
    }

    qint64 time_min = fs.m_minDateTime.toMSecsSinceEpoch();
    qint64 time_delta = fs.m_minDateTime.msecsTo(fs.m_maxDateTime);

    qreal temperature_delta = temperatureHighBound - temperatureLowBound;
    qreal current_delta = currentHighBound - currentLowBound;
    qreal voltage_delta = voltageHighBound - voltageLowBound;

    bool first = true;
    QPointF last_p_t, last_p_c, last_p_v;

    QTextStream in(&inputFile);
    QString temp;
    while(!(temp = in.readLine()).isNull()) {
        QStringList parts = temp.split(';');
        QDateTime dt = QDateTime::fromString(parts[0], "yyyyMMdd'T'hhmmss");
        qreal t = parts[1].toDouble();
        qreal c = parts[2].toDouble();
        qreal v = parts[3].toDouble();

        qreal x = plot_width * (dt.toMSecsSinceEpoch() - time_min) / time_delta;

        qreal y_t = plot_height * (t - temperatureLowBound) / temperature_delta;
        qreal y_c = plot_height * (c - currentLowBound) / current_delta;
        qreal y_v = plot_height * (v - voltageLowBound) / voltage_delta;

        QPointF p_t(x + 25, 70  + plot_height - y_t);
        QPointF p_c(x + 25, 140 + plot_height - y_c);
        QPointF p_v(x + 25, 210 + plot_height - y_v);

        if (first) {
            first = false;
        } else {
            _setPen(0xFF0000);
            _drawLine(last_p_t.x(), last_p_t.y(), p_t.x(), p_t.y());

            _setPen(0x00FF00);
            _drawLine(last_p_c.x(), last_p_c.y(), p_c.x(), p_c.y());

            _setPen(0x0000FF);
            _drawLine(last_p_v.x(), last_p_v.y(), p_v.x(), p_v.y());
        }

        last_p_t = p_t;
        last_p_c = p_c;
        last_p_v = p_v;
    }
    inputFile.close();

    p.end();

    return "";
}

int ReportGenerator::convertX(qreal x_mm) {
    static const qreal wPage = (300.0 * 8.5);
    return (int)(x_mm * wPage / 215.9);
}
int ReportGenerator::convertY(qreal y_mm) {
    static const qreal hPage = (300.0 * 11.0);
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
QRectF ReportGenerator::convert(const QRectF &r) {
    return QRectF(convertX(r.x()), convertY(r.y()), convertW(r.width()), convertH(r.height()));
}

void ReportGenerator::_setPen(int hex) {
    m_printer->setPen(QColor(QRgb(hex)));
}
void ReportGenerator::_setBrush(int hex) {
    m_printer->setBrush(QColor(QRgb(hex)));
}
void ReportGenerator::_setFont(int size, const QString &fontName) {
    m_printer->setFont(QFont(fontName, size));
}
void ReportGenerator::_fillRect(const QRectF &bounds_mm) {
    m_printer->fillRect(convertX(bounds_mm.x()), convertY(bounds_mm.y()), convertW(bounds_mm.width()), convertH(bounds_mm.height()), m_printer->brush());
}
void ReportGenerator::_drawRect(const QRectF &bounds_mm) {
    QBrush q = m_printer->brush();
    m_printer->setBrush(Qt::NoBrush);
    m_printer->drawRect(convertX(bounds_mm.x()), convertY(bounds_mm.y()), convertH(bounds_mm.width()), convertH(bounds_mm.height()));
    m_printer->setBrush(q);
}
void ReportGenerator::_drawPixmap(const QRectF &bounds_mm, const QString &resourceName) {
    m_printer->drawPixmap(convertX(bounds_mm.x()), convertY(bounds_mm.y()), convertW(bounds_mm.width()), convertH(bounds_mm.height()), QPixmap(resourceName));
}
void ReportGenerator::_drawText(const QRectF& boundingBox, const QString &text, int flags) {
    m_printer->drawText(convert(boundingBox), flags, text);
}

void ReportGenerator::_drawLine(double x1_mm, double y1_mm, double x2_mm, double y2_mm) {
    m_printer->drawLine(QLineF(convertX(x1_mm), convertY(y1_mm), convertX(x2_mm), convertY(y2_mm)));
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
        qreal wi = colWidths[i];
        for (int j = 0; j < numRows; j++) {
            _drawText(QRectF(x_mm+xGap, y_mm+j*cellHeight, wi, cellHeight), rows[j][i]);
        }
        xGap += wi;
    }
}

void ReportGenerator::_drawFrameGraph(const QRectF &bounds_mm, const QList<double> &yLabels, const QList<QDateTime> &xLabels) {
    if (bounds_mm.x() < 0 || bounds_mm.y() < 0 || bounds_mm.width() == 0 || bounds_mm.height() == 0) {
        qDebug() << "Invalid QRectF values:"
            << bounds_mm.x() << "," << bounds_mm.y()
            << " - "
            << bounds_mm.width() << "x" << bounds_mm.height()
        ;
        return;
    }
    _drawRect(bounds_mm);

    static const QString d_format1 = "yyyy-MM-dd";
    static const QString d_format2 = "hh:mm:ss";

    if (yLabels.size() > 1) {
        static const qreal y_tick_length = 1.0;
        static const qreal bb_h = 4.0;
        static const qreal bb_w = 10.0;

        qreal bb_x = bounds_mm.x();
        for (int i = 0; i < yLabels.size(); i++) {
            qreal bb_y = (bounds_mm.height() + bounds_mm.y()) - (bounds_mm.height() * i) / (yLabels.size() - 1);
            _drawLine(bb_x - y_tick_length, bb_y, bb_x, bb_y);

            QRectF rect(bb_x - bb_w - y_tick_length - 0.5, bb_y - bb_h / 2.0, bb_w, bb_h);
            _drawText(rect, QString::number(yLabels[i], 'f', 1), Qt::AlignVCenter | Qt::AlignRight);
            //_drawRect(rect);
        }
    }

    if (xLabels.size() > 1) {
        static const qreal x_tick_length = 1.0;

        qreal wGap = bounds_mm.width() / (xLabels.size() - 1);
        qreal bb_y = bounds_mm.y() + bounds_mm.height();
        qreal bb_h = 4.0;
        for (int i = 0; i < xLabels.size(); i++) {
            qreal bb_x = bounds_mm.x() + i * wGap ;
            _drawLine(bb_x, bb_y, bb_x, bb_y + x_tick_length);

            QRectF rect1(bb_x - wGap / 2.0, bb_y + bb_h + x_tick_length + 0.5, wGap, bb_h);
            _drawText(rect1, xLabels[i].toString(d_format1), Qt::AlignCenter);
            //_drawRect(rect1);

            QRectF rect2(bb_x - wGap / 2.0, bb_y + x_tick_length + 0.5, wGap, bb_h);
            _drawText(rect2, xLabels[i].toString(d_format2), Qt::AlignCenter);
            //_drawRect(rect2);
        }
    }
}

qreal ReportGenerator::makeUpperBound(qreal value, qreal step) {
    int sign = (value < 0 ? -1 : +1);
    qreal fvalue = qAbs(value);
    return sign * ceil(fvalue/step) * step;
}
qreal ReportGenerator::makeLowerBound(qreal value, qreal step) {
    int sign = (value < 0 ? -1 : +1);
    qreal fvalue = qAbs(value);
    return sign * floor(fvalue/step) * step;
}
QList<qreal> ReportGenerator::splitSpan(qreal _min, qreal _max, int n) {
    qreal delta = _max - _min;
    QList<qreal> r;
    for (int i = 0; i <= n; i++) {
        r.append(_min + (delta * i) / n);
    }
    return r;
}
QList<QDateTime> ReportGenerator::splitSpan(const QDateTime &_min, const QDateTime &_max, int n) {
    qint64 delta = _min.msecsTo(_max);
    qint64 min_msecs = _min.toMSecsSinceEpoch();
    QList<QDateTime> r;
    for (int i = 0; i <= n; i++) {
        r.append(QDateTime::fromMSecsSinceEpoch(min_msecs + delta / n * i));
    }
    return r;
}
