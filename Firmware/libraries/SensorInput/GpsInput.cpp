#include <GpsInput.h>

GpsStruct::GpsStruct() :
    year(0), month(0), day(0), hour(0), minute(0), second(0),
    fix(0), quality(0),

    int_lat(0),      dec_lat(0),
    int_lon(0),      dec_lon(0),
    int_speed(0),    dec_speed(0),
    int_angle(0),    dec_angle(0),
    int_altitude(0), dec_altitude(0),

    flat(-1), flon(-1), fspeed(-1), fangle(-1), faltitude(-1), satellites(0)
{
}

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

            m_data.flat = (m_data.fix ? m_gps.latitude : 0);
            m_data.flon = (m_data.fix ? m_gps.longitude : 0);
            m_data.fspeed = (m_data.fix ? m_gps.speed : 0);
            m_data.faltitude = (m_data.fix ? m_gps.altitude : 0);
            m_data.fangle = (m_data.fix ? m_gps.angle : 0);

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
