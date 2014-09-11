#include <FirmwareSlave.h>

FirmwareSlave::FirmwareSlave() {
}

void FirmwareSlave::setup(int rx1, int tx1, int rx2, int tx2, int currentSensorPin,
        int voltageSensorPin, int bauds) {
    pinMode(13, OUTPUT);

    m_eeprom_writer.read(m_id);

    m_dex.setup(m_id, this);
    m_dex.setupHardware(&m_dsr_a, &m_dsw_a);
    m_dex.setupSoftware(&m_dsr_b, &m_dsw_b);

    m_comm_a.setup(rx1, tx1, bauds);
    m_comm_b.setup(rx2, tx2, bauds);

    m_comm_a.waitForConnection();
    m_comm_b.waitForConnection();
}

void FirmwareSlave::loop() {
    m_dex.loop();
}

bool FirmwareSlave::handleMessage(Message *message) {
    const int led = 13;
    for(int i=0; i<3; i++) {
        digitalWrite(led, HIGH);
        delay(100);
        digitalWrite(led, LOW);
        delay(100);
    }
    return false;
}
