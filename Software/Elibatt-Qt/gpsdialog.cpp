#include "gpsdialog.h"

GpsDialog::GpsDialog()
    : m_ui(new Ui::GpsDialog)
{
    m_ui->setupUi(this);

    connect(m_ui->zoom, SIGNAL(valueChanged(int)), this, SLOT(applyZoom()));
}

GpsDialog::~GpsDialog() {
    delete m_ui;
}

void GpsDialog::setLocation(double lat, double lon) {
    m_lat = lat;
    m_lon = lon;
}

int GpsDialog::exec() {
    m_ui->zoom->setValue(16);
    applyZoom();
    return QDialog::exec();
}

void GpsDialog::applyZoom() {
    m_ui->gmap->stop();

    int level = m_ui->zoom->value();
    QString url = QString("http://maps.google.com/maps/api/staticmap?center=%1,%2&zoom=%3&markers=%1,%2&size=500x500&sensor=false")
            .arg(m_lat).arg(m_lon).arg(level);
    qDebug() << "url:" << url;
    m_ui->gmap->load(QUrl(url));
}
