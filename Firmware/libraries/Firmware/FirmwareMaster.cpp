#include <FirmwareMaster.h>

/** ----------- RTC specific code ----------- */
volatile long TOGGLE_COUNT = 0;

ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}

ISR(INT0_vect) {
}

/** ------------------------------------------*/

void FirmwareMaster::propagateForward(Message &msg) {
    //msg.writeTo(&m_dsw_a); m_dsw_a.flush();
    msg.writeTo(&m_dsw_b); m_dsw_b.flush();
}

void FirmwareMaster::propagateBackward(Message &msg) {
    msg.writeTo(&m_dsw_a); m_dsw_a.flush();
}

void FirmwareMaster::set(Message &msg, uint32_t from, uint32_t to, byte type) {
    msg.m_fromId = from;
    msg.m_targetId = to;
    msg.m_type = type;
}

bool FirmwareMaster::printDbg(const char *s) {
    Message msg;
    msg.m_fromId = m_id;
    msg.m_targetId = 1234;
    msg.m_type = 255;
    msg.clearData();
    for (int i = 0; i < 8 && s[i]; i++) {
        msg.m_data[i] = s[i];
    }
    msg.writeTo(&m_dsw_a); m_dsw_a.flush();
    return false;
}

FirmwareMaster::FirmwareMaster() {
}

void FirmwareMaster::setup(int dbg_rx, int dbg_tx, int dbg_bauds, int rx2, int tx2, int bauds2,
        int currentSensorPin, int voltageSensorPin, int sdCsPin, uint32_t fileDuration,
        int debugPin, SerialEndpoint *dbgEndpoint) {
    Firmware::setup(dbg_rx, dbg_tx, dbg_bauds, rx2, tx2, bauds2, currentSensorPin, voltageSensorPin,
            debugPin, dbgEndpoint);

    d.println(F("Setting Bank Data (card and registered slaves):"));
    if ( m_bank_data.setup(sdCsPin, fileDuration, dbgEndpoint) || true) {
        // TODO(rtapiapincheira): discover new slaves and update eeprom list
        m_bank_data.registerId(m_id);
    } else {
        d.println(F("Error"));
        Utils::onFailure(debugPin, F("SD can't be setup, check wiring."));
    }

    d.print(F("Setting RTC: "));
    if (m_rtc_clock.setup(dbgEndpoint)) {
        d.println("OK!");
    } else {
        d.println("Error");
        Utils::onFailure(debugPin, F("RTC can't be setup, check wiring"));
    }

    d.println(F("Starting MASTER main loop"));
}

void FirmwareMaster::loop() {
    m_dex.loop();
}

bool FirmwareMaster::handleMessage(Message &message) {
    int remaining;
    byte slavesCount;
    uint16_t temp;
    DateTime now;
    bool s;
    uint32_t pcId;

    switch (message.m_type) {
    case SCAN_MESSAGE_RESPONSE:
        d.println(F("SCAN_MESSAGE_RESPONSE:"));
        s = m_bank_data.registerId(message.m_fromId);
        break;

    case SCAN_MESSAGE_START:
        d.println(F("SCAN_MESSAGE_START:"));

        pcId = message.m_fromId;

        message.clearData();
        set(message, m_id, 0, SCAN_MESSAGE);
        propagateForward(message);

        message.m_targetId = pcId;
        message.m_type = SCAN_MESSAGE_START_RESPONSE;
        return true;

    case MASTER_DATA_READ:
        d.println(F("MASTER_DATA_READ:"));
        Utils::toByte(temp = (uint16_t)m_temp_sensor.readDigital(), message.m_data);
        d.print(F("t=")).println(temp);

        Utils::toByte(temp = (uint16_t)m_current_sensor.read(), message.m_data+2);
        d.print(F("i=")).println(temp);

        Utils::toByte(temp = (uint16_t)m_voltage_sensor.read(), message.m_data+4);
        d.print(F("v=")).println(temp);

        set(message, m_id, message.m_fromId, MASTER_DATA_READ_RESPONSE);

        return true;
    case MASTER_DATA_GATHER:
        d.println(F("MASTER_DATA_GATHER:"));

        now = m_rtc_clock.now();

        m_bank_data.setTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
        remaining = m_bank_data.addData(
            m_id,
            m_temp_sensor.readDigital(),
            m_current_sensor.read(),
            m_voltage_sensor.read()
        );

        // Do not emit a response until all data is gathered first!
        if (remaining > 0) { // this handles the case when no slave are connected to master
            d.print(F("Emitting SLAVE_DATA_READ command to"));
            // Emit one SLAVE_DATA_READ message to every connected slave
            set(message, m_id, 0, SLAVE_DATA_READ);
            message.clearData();
            slavesCount = m_bank_data.registeredIdCount();
            d.print((uint32_t)slavesCount).println(" slaves");
            for (byte i = 0; i < slavesCount; i++) {
                message.m_targetId = m_bank_data.registeredIdAt(i);
                propagateForward(message);
            }
        } else {
            set(message, m_id, message.m_fromId, MASTER_DATA_GATHER_RESPONSE);
            message.clearData();
            message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
            return true;
        }
        break;
    case SLAVE_DATA_READ_RESPONSE:
        d.println(F("SLAVE_DATA_READ_RESPONSE:"));

        // When the last requested SLAVE_DATA_READ response is processed, the file will be
        // automatically flushed to a new csv row.
        remaining = m_bank_data.addData(
            message.m_fromId,
            message.m_data
        );
        if (remaining == 0) {
            set(message, m_id, 1234, MASTER_DATA_GATHER_COMPLETE);
            message.clearData();
            message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
            propagateBackward(message);
            return false;
        }
        break;
    case MASTER_RTC_TIME_GET:
        break;
    case MASTER_RTC_TIME_SET:

        break;
    case MASTER_ID_WRITE:
        d.println(F("MASTER_ID_WRITE"));
        m_eeprom_writer.writeId(Utils::toInt32(message.m_data));
        message.clearData();
        message.m_data[CUSTOM_MESSAGE_DATA_LENGTH-1] = 1;
        message.m_type = MASTER_ID_WRITE_RESPONSE;
        message.m_targetId = message.m_fromId;
        message.m_fromId = m_id;
        return true;
    }
    return false;
}

void FirmwareMaster::process(char cmd) {
    Message msg;
    switch(cmd) {
    case 'g':
        msg.m_type = MASTER_DATA_GATHER;
        msg.clearData();
        msg.m_fromId = m_id;
        msg.m_targetId = m_id;
        m_dex.injectMessage(msg);
        break;
    case 'r':
        msg.m_type = MASTER_DATA_READ;
        msg.clearData();
        msg.m_fromId = m_id;
        msg.m_targetId = m_id;
        m_dex.injectMessage(msg);
        break;
    case 's':
        msg.m_type = SCAN_MESSAGE;
        msg.clearData();
        msg.m_fromId = m_id;
        msg.m_targetId = m_id;
        m_dex.injectMessage(msg);
        break;
    default:
        break;
    }
}
