#include <V2Libs.h>

//#define OUTPUT_TO_PC

#define NO_DATA "-1"

float V2Libs::toAmps(float vout3v) {
  return (36.7f * vout3v) / MAX_VOLTS - 18.3f;
}

float V2Libs::toVolts(int reading) {
  return (MAX_VOLTS * reading) / 1023;
}

int V2Libs::sendThrough(GpsStruct &gdata, HardwareSerial *se) {
    return
        se->print("{'GPS':[{'Fix':'") +
        se->print(gdata.fix ? "1" : "0") +
        se->print("','Latitud':'") + se->print((float)(gdata.fix ? gdata.flat : -1)) +
        se->print("','Longitud':'") + se->print((float)(gdata.fix ? gdata.flon : -1)) +
        se->print("','Altura':'") + se->print((float)(gdata.fix ? gdata.faltitude : -1)) +
        se->println("'}]}");
}

int V2Libs::sendAnalogValues(HardwareSerial *se) {
    int size =
        se->print("S:")+
        se->print(getTemperature()) + se->print(";") +
        se->print(getCurrent()) + se->print(";") +
        se->print(getVoltage()) + se->println();
    se->flush();
    return size;
}

V2Libs::V2Libs() : BANK_COMM(10,11) {
    pcCommEnabled = false;
    bleCommEnabled = false;
    bankCommEnabled = false;
    tempSensorEnabled = false;
    gpsEnabled = false;
    sdWriterEnabled = false;
    rtcClockEnabled = false;
}

V2Libs::~V2Libs() {
}

void V2Libs::maybeDebug(const char *s) {
#if RELEASE_BOARD
#else
    BLE_COMM.println(s);
    BLE_COMM.flush();
#endif
}

void V2Libs::setupPcComm() {
    PC_COMM.begin(57600); // Serial  @ 57600
    while(!PC_COMM);
    maybeDebug("PC connected OK!");
    pcCommEnabled = true;
}
void V2Libs::setupBleComm() {
    BLE_COMM.begin(57600);  // Serial3 @ 57600
    while(!BLE_COMM);
    maybeDebug("BLE connected OK!");
    bleCommEnabled = true;
}
void V2Libs::setupBankComm() {
    BANK_COMM.begin(9600);
    //while(!BANK_COMM);
    maybeDebug("BANK connected OK!");
    bankCommEnabled = true;
}
void V2Libs::setupParserComm() {
    parserComm.setup(&m_bank);
}
void V2Libs::setupEepromWriter() {
    eepromId = eepromWriter.readId();
}
void V2Libs::setupTempSensor() {
    if (!tempSensor.setup()) {
        maybeDebug("Couldn't find MCP9808!");
    }
    maybeDebug("MCP9808 setup OK!");
    tempSensorEnabled = true;
}
void V2Libs::setupCurrentSensor() {
    currentSensor.setup(A0);
    maybeDebug("Current sensor OK!");
}
void V2Libs::setupVoltageSensor() {
    voltageSensor.setup(A1);
    maybeDebug("Voltage divider OK!");
}
void V2Libs::setupGps() {
    gpsInput.setup(&Serial2, 9600);
    maybeDebug("GPS connected OK!");
    gpsEnabled = true;
}
void V2Libs::setupSdWriter() {
    if (!sdWriter.setup(38, NULL)) {
        maybeDebug("Error while connecting to SD.");
        while(1);
    }
    rowsCount = 0;
    maybeDebug("SD setup OK");
    sdWriterEnabled = true;
}
void V2Libs::setupRtcClock() {
    if (!rtcClock.setup(NULL)) {
        maybeDebug("Could not initialize RTC!");
        while(1);
    }
    maybeDebug("RTC setup OK!");
    rtcClockEnabled = true;
}

float V2Libs::getTemperature() {
    return tempSensor.readCelsius(); // Grados Celcius
}

float V2Libs::getCurrent() {
    return toAmps(toVolts(currentSensor.read()));
}

float V2Libs::getVoltage() {
  return (toVolts(voltageSensor.read()) * R1_R2) / R2;
}

GpsStruct V2Libs::getGpsStruct() {
    if (gpsInput.available()) {
        return gpsInput.getData();
    }
    return GpsStruct();
}

float V2Libs::getLatitude() {
    if (gpsInput.available()) {
        return gpsInput.getData().flat;
    }
    return 0;
}

float V2Libs::getLongitude() {
    if (gpsInput.available()) {
        return gpsInput.getData().flon;
    }
    return 0;
}

float V2Libs::getAltitude() {
    if (gpsInput.available()) {
        return gpsInput.getData().faltitude;
    }
    return 0;
}

DateTime V2Libs::getDateTime() {
    if (rtcClockEnabled && rtcClock.isAllSetUp()) {
        return rtcClock.now();
    }
    return DateTime();
}

float V2Libs::getAverageTemperature() {
    float avg = 0.0f;
    for (int i = 0; i < parserComm.slaves.n; i++) {
        avg += parserComm.slaves.temperature[i];
    }
    return avg/parserComm.slaves.n;
}
float V2Libs::getAverageCurrent() {
    float avg = 0.0f;
    for (int i = 0; i < parserComm.slaves.n; i++) {
        avg += parserComm.slaves.current[i];
    }
    return avg/parserComm.slaves.n;
}
float V2Libs::getAverageVoltage() {
    float avg = 0.0f;
    for (int i = 0; i < parserComm.slaves.n; i++) {
        avg += parserComm.slaves.voltage[i];
    }
    return avg/parserComm.slaves.n;
}
int V2Libs::getSlaveCount() {
    return parserComm.slaves.n;
}

int V2Libs::sendTemperatureList(HardwareSerial *se) {
    int size = se->print("TS:");
    for (int i = 0; i < parserComm.slaves.n; i++) {
        if (i > 0) {
            size += se->print(";");
        }
        size += se->print(parserComm.slaves.temperature[i]);
    }
    size += se->println();
    se->flush();
    return size;
}
int V2Libs::sendCurrentList(HardwareSerial *se) {
    int size = se->print("CS:");
    for (int i = 0; i < parserComm.slaves.n; i++) {
        if (i > 0) {
            size += se->print(";");
        }
        size += se->print(parserComm.slaves.current[i]);
    }
    size += se->println();
    se->flush();
    return size;
}
int V2Libs::sendVoltageList(HardwareSerial *se) {
    int size = se->print("VS:");
    for (int i = 0; i < parserComm.slaves.n; i++) {
        if (i > 0) {
            size += se->print(";");
        }
        size += se->print(parserComm.slaves.voltage[i]);
    }
    size += se->println();
    se->flush();
    return size;
}
int V2Libs::sendIdList(HardwareSerial *se) {
    int size = se->print("IDS:");
    for (int i = 0; i < parserComm.slaves.n; i++) {
        if (i > 0) {
            size += se->print(";");
        }
        size += se->print(parserComm.slaves.id[i]);
    }
    size += se->println();
    se->flush();
    return size;
}

void V2Libs::setup() {
#if RELEASE_BOARD
#else
    setupPcComm();
#endif
    setupBleComm();
    setupBankComm();
    setupParserComm();
    setupEepromWriter();
    setupTempSensor();
    setupCurrentSensor();
    setupVoltageSensor();
//    setupGps();
//    setupSdWriter();
//    setupRtcClock();

    parserComm.putMasterInfo(eepromId, getTemperature(), getCurrent(), getVoltage());
}

void V2Libs::loop() {
    if (gpsEnabled) {
        gpsInput.loop();
    }

    if (bankCommEnabled) {
        parserComm.loop();
    }

    static char cmdBuffer[BUFFER_MAX];
    static int cmdLen = 0;

    GpsStruct gdata;

    if (bleCommEnabled) {
        if (BLE_COMM.available()) {
            char c = BLE_COMM.read();
            cmdBuffer[cmdLen++] = c;

            if (cmdLen >= BUFFER_MAX || cmdBuffer[cmdLen-1] == '\n') {
                int command = cmd.parseCmd(cmdBuffer, cmdLen);

                //Serial.print("command:");
                //Serial.println(command);

                float value;

                switch(command) {
                case CMD_TEMPERATURE:
                    BLE_COMM.print("T:");
                    BLE_COMM.println(getTemperature());
                    break;

                case CMD_CURRENT:
                    BLE_COMM.print("C:");
                    BLE_COMM.println(getCurrent());
                    break;

                case CMD_VOLTAGE:
                    BLE_COMM.print("V:");
                    BLE_COMM.println(getVoltage());
                    break;

                case CMD_GPS:
                    gdata = getGpsStruct();
                    sendThrough(gdata, &BLE_COMM);
                    break;

                case CMD_LATITUDE:
                    BLE_COMM.print("LA:");
                    BLE_COMM.println(getLatitude());
                    break;

                case CMD_LONGITUDE:
                    BLE_COMM.print("LO:");
                    BLE_COMM.println(getLongitude());
                    break;

                case CMD_HEIGHT:
                    BLE_COMM.print("H:");
                    BLE_COMM.println(getAltitude());
                    break;

                case CMD_SENSORS:
                    sendAnalogValues(&BLE_COMM);
                    break;

                case CMD_ID:
                    BLE_COMM.print("ID:");
                    BLE_COMM.println(eepromId);
                    break;

                case CMD_AVG_TEMPERATURE:
                    BLE_COMM.print("TX:");
                    BLE_COMM.println(getAverageTemperature());

                    // Update temperatures of all boards
                    parserComm.slaves.temperature[0] = getTemperature();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;
                case CMD_AVG_CURRENT:
                    BLE_COMM.print("CX:");
                    BLE_COMM.println(getAverageCurrent());

                    // Update current of all boards
                    parserComm.slaves.current[0] = getCurrent();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;
                case CMD_AVG_VOLTAGE:
                    BLE_COMM.print("VX:");
                    BLE_COMM.println(getAverageVoltage());

                    // Update voltage of all boards
                    parserComm.slaves.voltage[0] = getVoltage();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;

                case CMD_LIST_SIZE:
                    BLE_COMM.print("N:");
                    BLE_COMM.println(getSlaveCount());
                    break;

                case CMD_LIST_TEMPERATURE:
                    sendTemperatureList(&BLE_COMM);

                    // Update temperature of all boards
                    parserComm.slaves.temperature[0] = getTemperature();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;
                case CMD_LIST_CURRENT:
                    sendCurrentList(&BLE_COMM);

                    // Update current of all boards
                    parserComm.slaves.current[0] = getCurrent();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;
                case CMD_LIST_VOLTAGE:
                    sendVoltageList(&BLE_COMM);

                    // Update voltage of all boards
                    parserComm.slaves.voltage[0] = getVoltage();
                    if (bankCommEnabled) {
                        BANK_COMM.println("S");
                    }

                    break;
                case CMD_LIST_ID:
                    sendIdList(&BLE_COMM);
                    break;

                default:
                    BLE_COMM.println("Command not found!");
                    maybeDebug("Command not found!");
                    break;
                }

                cmdLen = 0;
            }
        } // endif ble.available()
    } // endif bleCommEnabled

    // ########################## Sensors Handling ####################
    /*static long last_t = 0;
    if (millis() - last_t >= 1000) {
        DateTime rtc_now = getDateTime();

        float c = getTemperature();
        float current = getCurrent();
        float realv = getVoltage();

    #ifdef OUTPUT_TO_PC
        pcComm.print("Temp: "); pcComm.print(c); pcComm.print("*C\t");
        pcComm.print(f); pcComm.println("*F");
        pcComm.print("Corriente: "); pcComm.print(current); pcComm.println("[A]");
        pcComm.print("Voltaje: "); pcComm.print(realv); pcComm.println("[V]");
    #endif

    // ########################## GPS Handling ####################

    GpsStruct gdata = getGpsStruct();

    #ifdef OUTPUT_TO_PC

        pcComm.print("Time: ");
        pcComm.printlnSimpleTime(gdata.hour, gdata.minute, gdata.second);

        pcComm.print("Date: ");
        pcComm.printlnSimpleDate(gdata.day, gdata.month, gdata.year);

        pcComm.print("Fix: "); pcComm.print((int)gdata.fix);
        pcComm.print(" quality: "); pcComm.println((int)gdata.quality);

        if (gdata.fix) {
            pcComm.print("Location: ");
            pcComm.print(gdata.flat);
            pcComm.print(", ");
            pcComm.print(gdata.flon);

            pcComm.print("Speed (knots): "); pcComm.println(gdata.fspeed);
            pcComm.print("Angle: ");         pcComm.println(gdata.fangle);
            pcComm.print("Altitude: ");      pcComm.println(gdata.faltitude);
            pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
        }
    #endif

        if (sdWriterEnabled) {
            // Inicializar archivo Tarjeta Sd
            if (rowsCount < 0) {
                maybeDebug("Omitiendo escritura de archivo, dado que hay error previo...");
            } else {
                // For new empty files, generate
                if (rowsCount == 0) {
                    if(!sdWriter.open()) {
                        rowsCount = -1;
                    }
                }

                // Write data to file if no error is present
                if (rowsCount >= 0) {
                    sdWriter.writeDatetime(rtc_now.year(), rtc_now.month(), rtc_now.day(), rtc_now.hour(), rtc_now.minute(), rtc_now.second());
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(c);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(current);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(realv);
                    sdWriter.writeChar(';');

                    // GPS Data

                    sdWriter.writeChar(gdata.fix ? '1' : '0');
                    sdWriter.writeChar(';');

                    sdWriter.writeDatetime(gdata.year, gdata.month, gdata.day, gdata.hour, gdata.minute, gdata.second);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(gdata.flat);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(gdata.flon);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(gdata.fspeed);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(gdata.fangle);
                    sdWriter.writeChar(';');

                    sdWriter.writeFloat(gdata.faltitude);
                    sdWriter.writeChar(';');

                    sdWriter.writeInt32(gdata.satellites);
                    sdWriter.writeChar(';');

                    sdWriter.writeNewline();

                    rowsCount++;
                }

                // Signal to renew the file in the next iteration.
                if (rowsCount == MAX_ROWS) {
                    rowsCount = 0;
                }
            }
        }

        last_t = millis();
    }*/
}

