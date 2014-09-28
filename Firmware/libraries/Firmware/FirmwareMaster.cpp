#include <FirmwareMaster.h>

void FirmwareMaster::propagateMessage() {
    Serial.println("Sending message through both endpoints");
    m_the_message.writeTo(&m_dsw_a); m_dsw_a.flush();
    m_the_message.writeTo(&m_dsw_b); m_dsw_b.flush();
}

FirmwareMaster::FirmwareMaster() {
}

void FirmwareMaster::setup(int rx2, int tx2, int bauds2, int currentSensorPin, int voltageSensorPin,
        int sdCsPin, uint32_t fileDuration, int debugPin, SerialEndpoint *dbgEndpoint) {
    Firmware::setup(-1, -1, 0, rx2, tx2, bauds2, currentSensorPin, voltageSensorPin, debugPin,
    		dbgEndpoint);

    d.println("Setting Bank Data (card and registered slaves):");
    if (m_bank_data.setup(sdCsPin, fileDuration, dbgEndpoint)) {
    	// TODO(rtapiapincheira): discover new slaves and update eeprom list
    	m_bank_data.registerId(Utils::toInt32(m_id));
    	//m_bank_data.registerId(Utils::toInt32("0002"));
    	//m_bank_data.registerId(Utils::toInt32("0003"));
        d.println("Ok");
    } else {
        d.println("Error");
        Utils::onFailure("SD can't be setup, check wiring.");
    }

    d.println("Starting MASTER main loop");
}

void FirmwareMaster::loop() {
    m_dex.loop();
}

bool FirmwareMaster::handleMessage(Message &message) {
    switch (message.m_type) {
    case MASTER_DATA_READ:
    	break;
    case MASTER_DATA_GATHER:
        uint16_t val1 = Utils::toShort(message.m_data[2], message.m_data[3]);
        uint16_t val2 = Utils::toShort(message.m_data[4], message.m_data[5]);
        uint16_t val3 = Utils::toShort(message.m_data[6], message.m_data[7]);

        d
        .print("temperature=").print(val1) // TOOD(rtapiapincheira): convert this value from digital to celsius
        .print(";current=").print(val2)
        .print(";voltage=").print(val3)
        .println();

        /*
        id = Utils::toInt32(m_id);

        m_bank_data.setTime(2014, 9, 20, 22, 15, 10);

        tempVal = m_temp_sensor.readDigital();
        currentVal = m_current_sensor.read();
        voltageVal = m_voltage_sensor.read();

        m_bank_data.addData(id, tempVal, currentVal, voltageVal);
        */

    	break;
    case MASTER_GPS_GET:
    	break;
    case MASTER_RTC_GET:
    	break;
    case MASTER_RTC_SET:
    	break;
    case MASTER_ID_WRITE:
    	break;
    }
    return true;
}
