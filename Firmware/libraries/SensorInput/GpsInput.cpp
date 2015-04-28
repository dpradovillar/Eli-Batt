#include <GpsInput.h>

GpsInput::GpsInput() : m_ok (false)
{
}

bool GpsInput::setup(HardwareSerial *serial, int gps_bauds) {
    m_gps.setup(serial);

    m_gps.begin(gps_bauds);
    m_gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    m_gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    m_gps.sendCommand(PGCMD_ANTENNA);

    m_ok = true;

    return m_ok;
}

void GpsInput::loop() {
    m_gps.read();
}

bool GpsInput::available() {
    if (m_gps.newNMEAreceived()) {
        if (m_gps.parse(m_gps.lastNMEA())) {
            m_data.year = 2000+m_gps.year;
            m_data.month = m_gps.month;
            m_data.day = m_gps.day;

            m_data.hour = m_gps.hour;
            m_data.minute = m_gps.minute;
            m_data.second = m_gps.seconds;

            m_data.fix = m_gps.fix;

            m_data.quality = m_gps.fixquality;

            Utils::splitFloat(m_gps.latitude, m_data.int_lat, m_data.dec_lat);
            Utils::splitFloat(m_gps.longitude, m_data.int_lon, m_data.dec_lon);
            Utils::splitFloat(m_gps.speed, m_data.int_speed, m_data.dec_speed);
            Utils::splitFloat(m_gps.altitude, m_data.int_altitude, m_data.dec_altitude);
            Utils::splitFloat(m_gps.angle, m_data.int_angle, m_data.dec_angle);

            m_data.satellites = m_gps.satellites;
        } else {
            return false;
        }
    }
    return true;
}

GpsStruct GpsInput::getData() {
    return m_data;
}
