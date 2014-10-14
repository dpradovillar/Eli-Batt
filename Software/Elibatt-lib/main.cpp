#include <iostream>
#include "threadedserial.h"
using namespace std;

int main(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    cout << "SCAN_MESSAGE                   1" << endl;
    cout << "SCAN_MESSAGE_RESPONSE        101" << endl;
    cout << "SCAN_MESSAGE_START             2" << endl;
    cout << "SCAN_MESSAGE_START_RESPONSE  102" << endl;
    cout << "SLAVE_ID_WRITE                 3" << endl;
    cout << "SLAVE_ID_WRITE_RESPONSE      103" << endl;
    cout << "SCAN_ID_READ                   4" << endl;
    cout << "SCAN_ID_READ_RESPONSE        104" << endl;
    cout << "SCAN_ID_CHECK                  5" << endl;
    cout << "SCAN_ID_CHECK_RESPONSE       105" << endl;
    cout << "SLAVE_DATA_READ                6" << endl;
    cout << "SLAVE_DATA_READ_RESPONSE     106" << endl;
    cout << "MASTER_DATA_READ               7" << endl;
    cout << "MASTER_DATA_READ_RESPONSE    107" << endl;
    cout << "MASTER_DATA_GATHER             8" << endl;
    cout << "MASTER_DATA_GATHER_RESPONSE  108" << endl;
    cout << "MASTER_DATA_GATHER_COMPLETE  208" << endl;
    cout << "MASTER_RTC_TIME_GET               9" << endl;
    cout << "MASTER_RTC_DATE_GET              10" << endl;
    cout << "MASTER_RTC_TIME_SET              11" << endl;
    cout << "MASTER_RTC_DATE_SET              12" << endl;
    cout << "MASTER_RTC_TIME_GET_RESPONSE    109" << endl;
    cout << "MASTER_RTC_DATE_GET_RESPONSE    110" << endl;
    cout << "MASTER_RTC_TIME_SET_RESPONSE    111" << endl;
    cout << "MASTER_RTC_DATE_SET_RESPONSE    112" << endl;
    cout << "MASTER_ID_WRITE                  13" << endl;
    cout << "MASTER_ID_WRITE_RESPONSE        113" << endl;
    cout << "MASTER_GPS_TIME_GET              14" << endl;
    cout << "MASTER_GPS_DATE_GET              15" << endl;
    cout << "MASTER_GPS_LAT_GET               16" << endl;
    cout << "MASTER_GPS_LON_GET               17" << endl;
    cout << "MASTER_GPS_ALTITUDE_GET          18" << endl;
    cout << "MASTER_GPS_FIX_TYPE_GET          19" << endl;
    cout << "MASTER_GPS_SPEED_GET             20" << endl;
    cout << "MASTER_GPS_TRACK_GET             21" << endl;
    cout << "MASTER_GPS_PDOP_GET              22" << endl;
    cout << "MASTER_GPS_HDOP_GET              23" << endl;
    cout << "MASTER_GPS_VDOP_GET              24" << endl;
    cout << "MASTER_GPS_ENABLED_GET           25" << endl;
    cout << "MASTER_GPS_TIME_GET_RESPONSE      114" << endl; // hour_byte, minute_byte, second_byte
    cout << "MASTER_GPS_DATE_GET_RESPONSE      115" << endl; // year_byte, month_byte, day_byte
    cout << "MASTER_GPS_LAT_GET_RESPONSE       116" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_LON_GET_RESPONSE       117" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_ALTITUDE_GET_RESPONSE  118" << endl; // altitude_int32
    cout << "MASTER_GPS_FIX_TYPE_GET_RESPONSE  119" << endl; // fixed_byte (0 or 1)
    cout << "MASTER_GPS_SPEED_GET_RESPONSE     120" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_TRACK_GET_RESPONSE     121" << endl; // tracking_byte
    cout << "MASTER_GPS_PDOP_GET_RESPONSE      122" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_HDOP_GET_RESPONSE      123" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_VDOP_GET_RESPONSE      124" << endl; // integer_int32, decimal_int32
    cout << "MASTER_GPS_ENABLED_GET_RESPONSE   125" << endl; // enabled_gps_byte
    cout << "MASTER_BLE_ENABLED_SET             26" << endl;
    cout << "MASTER_BLE_ENABLED_GET             27" << endl;
    cout << "MASTER_BLE_ENABLED_SET_RESPONSE   126" << endl; // enabled_byte
    cout << "MASTER_BLE_ENABLED_GET_RESPONSE   127" << endl; // enabled_byte

    cout << "<TYPE> <TARGET> [<B1> ... <B8>]" << endl;

    cout << flush;

    ThreadedSerial thread;
    thread.setParams("COM18", 9600);
    thread.start();

    QTextStream qtin(stdin);

    while (true) {
        QString line = qtin.readLine();
        if (line=="quit" || line=="exit" || line=="q") {
            break;
        }

        if (line.trimmed().isEmpty()) {
            continue;
        }

        cout << "line:{" << line.toStdString() << "}" << endl;
        QStringList parts = line.split(" ");

        Message m;
        m.m_crc = 0;
        m.m_type = parts.size() >= 1 ? parts[0].toInt() : 0;
        m.m_fromId = 1234;
        m.m_targetId = parts.size() >= 2 ? parts[1].toInt() : 0;

        m.m_data[0] = parts.size() >= 3 ? parts[2].toInt() : 0;
        m.m_data[1] = parts.size() >= 4 ? parts[3].toInt() : 0;
        m.m_data[2] = parts.size() >= 5 ? parts[4].toInt() : 0;
        m.m_data[3] = parts.size() >= 6 ? parts[5].toInt() : 0;
        m.m_data[4] = parts.size() >= 7 ? parts[6].toInt() : 0;
        m.m_data[5] = parts.size() >= 8 ? parts[7].toInt() : 0;
        m.m_data[6] = parts.size() >= 9 ? parts[8].toInt() : 0;
        m.m_data[7] = parts.size() >= 10 ? parts[9].toInt() : 0;

        thread.transmitPackage(m);
    }

    thread.stop();
    thread.wait();

    return 0;
}
