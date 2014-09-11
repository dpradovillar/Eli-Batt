#include <FirmwareMaster.h>

FirmwareMaster::FirmwareMaster() {
}

void FirmwareMaster::setup(int rx2, int tx2, int bauds,
        Endpoint *dbgEndpoint) {
    d.setup(dbgEndpoint);

    d.println("Reading ID from EEPROM");
    m_eeprom_writer.read(m_id);
    d.print("ID read from EEPROM: ").printInt(m_id).println();

    d.println("Setting First communication channel");
    m_dsr_a.setup(&m_comm_a);
	m_dsw_a.setup(&m_comm_a);
    m_comm_a.setup(-1, -1, bauds);
    m_comm_a.waitForConnection();

    d.println("Setting Second communication channel");
    m_dsr_b.setup(&m_comm_b);
	m_dsw_b.setup(&m_comm_b);
    m_comm_b.setup(rx2, tx2, bauds);
    m_comm_b.waitForConnection();

    d.println("Communications OK");

    d.println("Setting up communications");
    m_dex.setup(m_id, this);
    m_dex.setupHardware(&m_dsr_a, &m_dsw_a);
    m_dex.setupSoftware(&m_dsr_b, &m_dsw_b);

    d.println("Starting Master tasks");
}

void FirmwareMaster::loop() {
    m_dex.loop();
}

bool FirmwareMaster::handleMessage(Message *message) {
    return false;
}

void FirmwareMaster::process(int next) {
    d.print("Processing next:");
    d.println(next);
}
