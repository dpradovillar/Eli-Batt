#include "gpscontroller.h"

GpsController::GpsController()
{
}

GpsController::~GpsController()
{
}

void GpsController::setUi(Ui::UiDialog *ui) {
    m_ui = ui;

    connect(m_ui->view_in_maps, SIGNAL(clicked()), this, SLOT(showCoordinates()));
}

void GpsController::handleViewInMapsClick() {

}

void GpsController::showCoordinates() {
    double lat = m_ui->gps_lat->text().toDouble();
    double lon = m_ui->gps_lon->text().toDouble();

    GpsDialog diag;
    diag.setLocation(lat, lon);
    diag.exec();
}
