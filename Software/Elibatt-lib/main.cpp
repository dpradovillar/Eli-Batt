#include <iostream>
#include "threadedserial.h"
using namespace std;

int main(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    cout << "1   SCAN_MESSAGE" << endl;
    cout << "101 SCAN_MESSAGE_RESPONSE" << endl;
    cout << "2   SLAVE_ID_WRITE" << endl;
    cout << "102 SLAVE_ID_WRITE_RESPONSE" << endl;
    cout << "3   SCAN_ID_READ" << endl;
    cout << "103 SCAN_ID_READ_RESPONSE" << endl;
    cout << "4   SCAN_ID_CHECK" << endl;
    cout << "104 SCAN_ID_CHECK_RESPONSE" << endl;
    cout << "5   SLAVE_DATA_READ" << endl;
    cout << "105 SLAVE_DATA_READ_RESPONSE" << endl;
    cout << "6   MASTER_DATA_READ" << endl;
    cout << "106 MASTER_DATA_READ_RESPONSE" << endl;
    cout << "7   MASTER_DATA_GATHER" << endl;
    cout << "107 MASTER_DATA_GATHER_RESPONSE" << endl;
    cout << "8   MASTER_GPS_GET" << endl;
    cout << "108 MASTER_GPS_GET_RESPONSE" << endl;
    cout << "9   MASTER_RTC_SET" << endl;
    cout << "109 MASTER_RTC_SET_RESPONSE" << endl;
    cout << "10  MASTER_RTC_GET" << endl;
    cout << "110 MASTER_RTC_GET_RESPONSE" << endl;
    cout << "11  MASTER_ID_WRITE" << endl;
    cout << "111 MASTER_ID_WRITE_RESPONSE" << endl;
    cout << "12  SCAN_MESSAGE_START" << endl;
    cout << "112 SCAN_MESSAGE_START_RESPONSE" << endl;

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
