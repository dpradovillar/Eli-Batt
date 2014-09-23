#ifndef GPSDIALOG_H
#define GPSDIALOG_H

#include <QtCore>
#include <QDialog>
#include <QDebug>
#include "ui_gpsdialog.h"

class GpsDialog : public QDialog
{
    Q_OBJECT

    double m_lat;
    double m_lon;

    Ui::GpsDialog *m_ui;

public:
    explicit GpsDialog();
    virtual ~GpsDialog();

    void setLocation(double lat, double lon);

    int exec();

private slots:
    void applyZoom();
};

#endif // GPSDIALOG_H
