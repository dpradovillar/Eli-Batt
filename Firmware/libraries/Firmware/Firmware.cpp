#include <Firmware.h>
/*
void Firmware::propagateMessage() {
    d.println("Sending message through both endpoints");
    m_the_message.writeTo(&m_dsw_a); m_dsw_a.flush();
    m_the_message.writeTo(&m_dsw_b); m_dsw_b.flush();
}*/

void Firmware::packSensorValues(byte *buffer6bytes) {

}

Firmware::Firmware() :
	m_id(0)
{
}

Firmware::~Firmware() {
}

void Firmware::setup(
     int rx1, int tx1, int bauds1,
     int rx2, int tx2, int bauds2,
     int currentSensorPin, int voltageSensorPin,
     int debugPin, SerialEndpoint *additionalEndpoint, SerialEndpoint *dbgEndpoint)
{
    d.setup(dbgEndpoint);
    Wire.begin();

    pinMode(debugPin, OUTPUT);

    d.print(F("Reading ID from EEPROM:"));
    m_id = m_eeprom_writer.readId();
    d.print(m_id).print(F("/")).printHexUInt16(m_id).println();

    d.print(F("Setting Current Sensor:"));
    m_current_sensor.setup(currentSensorPin);
    d.println(F("Ok"));

    d.print(F("Setting Voltage Sensor:"));
    m_voltage_sensor.setup(voltageSensorPin);
    d.println(F("Ok"));

    d.print(F("Setting Temperature Sensor:"));
    if (m_temp_sensor.setup() || true) {
        d.println(F("Ok"));
    } else {
        d.println(F("Error"));
        Utils::onFailure(debugPin, F("Temperature sensor can't be setup, check wiring."));
    }

    m_dex.setup(m_id, this, dbgEndpoint);

    // DataExchanger will work OK
    if (rx1 < 0 || tx1 < 0) {
        d.println(F("Omitting First communication channel."));
    } else {
        d.print(F("Setting First communication channel:"));
        m_comm_a.setup(rx1, tx1, bauds1); // The master device has only one serial connection
        m_comm_a.waitForConnection();

        m_comm_a_wrapper.setup(&m_comm_a, additionalEndpoint);

        m_dsr_a.setup(&m_comm_a_wrapper);
        m_dsw_a.setup(&m_comm_a_wrapper);

        m_dex.setupHardware(&m_dsr_a, &m_dsw_a);
        d.println(F("Ok"));
    }

    if (rx2 < 0 || tx2 < 0) {
        d.println(F("Omitting Second communication channel."));
    } else {
        d.print(F("Setting Second communication channel:"));
        m_comm_b.setup(rx2, tx2, bauds2);
        m_comm_b.waitForConnection();

        m_comm_b_wrapper.setup(&m_comm_b, NULL);

        m_dsr_b.setup(&m_comm_b_wrapper);
        m_dsw_b.setup(&m_comm_b_wrapper);

        m_dex.setupSoftware(&m_dsr_b, &m_dsw_b);
        d.println(F("Ok"));
    }
}

void Firmware::loop() {
    m_dex.loop();
}
