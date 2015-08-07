#ifndef GPSCONTROLLER_H
#define GPSCONTROLLER_H

#include <QtCore>
#include "ui_uidialog.h"

#include "gpsdialog.h"

class GpsController : public QObject
{
    Q_OBJECT

    Ui::UiDialog *m_ui;

public:
    GpsController();
    ~GpsController();

    void setUi(Ui::UiDialog *ui);

private slots:
    void handleViewInMapsClick();
    void showCoordinates();
};

#endif // GPSCONTROLLER_H
