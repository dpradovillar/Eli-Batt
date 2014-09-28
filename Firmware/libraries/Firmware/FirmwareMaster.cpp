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
        m_bank_data.registerId(m_id);

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
    int remaining;
    byte slavesCount;

    switch (message.m_type) {
    case SCAN_MESSAGE_RESPONSE:
    	m_bank_data.registerId(message.m_fromId);
    	break;
    case MASTER_DATA_READ:
        Utils::toByte((uint16_t)m_temp_sensor.readDigital(), message.m_data);
        Utils::toByte((uint16_t)m_current_sensor.read(), message.m_data+2);
        Utils::toByte((uint16_t)m_voltage_sensor.read(), message.m_data+4);
        message.m_type = MASTER_DATA_READ_RESPONSE;
        return true;
    case MASTER_DATA_GATHER:
        m_bank_data.setTime(2014, 9, 20, 22, 15, 10);
        remaining = m_bank_data.addData(
            m_id,
            m_temp_sensor.readDigital(),
            m_current_sensor.read(),
            m_voltage_sensor.read()
        );
        // Do not emit a response until all data is gathered first!
        if (remaining > 0) { // this handles the case when no slave are connected to master
        	// Emit one SLAVE_DATA_READ message to every connected slave
			message.m_type = SLAVE_DATA_READ;
			message.clearData();
			message.m_fromId = m_id;
			slavesCount = m_bank_data.registeredIdCount();
			for(byte i = 0; i < slavesCount; i++) {
				message.m_targetId = m_bank_data.registeredIdAt(i);
				propagateMessage();
			}
        } else {
        	message.clearData();
			message.m_type = MASTER_DATA_GATHER_RESPONSE;
			message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
        	return true;
        }

        break;
    case SLAVE_DATA_READ_RESPONSE:
        // When the last requested SLAVE_DATA_READ response is processed, the file will be
        // automatically flushed to a new csv row.
        remaining = m_bank_data.addData(
            message.m_fromId,
            message.m_data
        );
        if (remaining == 0) {
        	message.clearData();
			message.m_type = MASTER_DATA_GATHER_RESPONSE;
			message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
			return true;
        }
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
    return false;
}
