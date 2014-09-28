#include <Firmware.h>

void Firmware::propagateMessage() {
    Serial.println("Sending message through both endpoints");
    m_the_message.writeTo(&m_dsw_a); m_dsw_a.flush();
    m_the_message.writeTo(&m_dsw_b); m_dsw_b.flush();
}

Firmware::Firmware() {
}

Firmware::~Firmware() {
}

void Firmware::setup(
     int rx1, int tx1, int bauds1,
     int rx2, int tx2, int bauds2,
     int currentSensorPin, int voltageSensorPin,
     int debugPin, SerialEndpoint *dbgEndpoint)
{
    d.setup(dbgEndpoint);

    pinMode(debugPin, OUTPUT);

    d.print("Reading ID from EEPROM:");
    m_eeprom_writer.read(m_id);
    d.printInt(m_id).print("/").printHexInt(m_id).println();

    d.print("Setting Current Sensor:");
    m_current_sensor.setup(currentSensorPin);
    d.println("Ok");

    d.print("Setting Voltage Sensor:");
    m_voltage_sensor.setup(voltageSensorPin);
    d.println("Ok");

    d.print("Setting Temperature Sensor:");
    if (m_temp_sensor.setup()) {
        d.println("Ok");
    } else {
        d.println("Error");
        Utils::onFailure("Temperature sensor can't be setup, check wiring.");
    }

    m_dex.setup(m_id, this, dbgEndpoint);

    // DataExchanger will work OK
    if (rx1==-1 || tx1==-1) {
        d.println("Omitting First communication channel.");
    } else {
        d.print("Setting First communication channel:");
        m_dsr_a.setup(&m_comm_a);
        m_dsw_a.setup(&m_comm_a);
        m_comm_a.setup(rx1, tx1, bauds1); // The master device has only one serial connection
        m_comm_a.waitForConnection();
        m_dex.setupHardware(&m_dsr_a, &m_dsw_a);
        d.println("Ok");
    }

    if (rx2==-1 || tx2==-1) {
        d.println("Omitting Second communication channel.");
    } else {
        d.print("Setting Second communication channel:");
        m_dsr_b.setup(&m_comm_b);
        m_dsw_b.setup(&m_comm_b);
        m_comm_b.setup(rx2, tx2, bauds2);
        m_comm_b.waitForConnection();
        m_dex.setupSoftware(&m_dsr_b, &m_dsw_b);
        d.println("Ok");
    }
}

void Firmware::loop() {
    m_dex.loop();
}
