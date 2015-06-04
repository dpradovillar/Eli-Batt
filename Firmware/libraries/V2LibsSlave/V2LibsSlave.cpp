#include <V2LibsSlave.h>

float V2LibsSlave::toAmps(float vout3v) {
  return (36.7f * vout3v) / MAX_VOLTS - 18.3f;
}

float V2LibsSlave::toVolts(int reading) {
  return (MAX_VOLTS * reading) / 1023;
}

int V2LibsSlave::sendAnalogValues(HardwareSerial *se) {
    int size =
        se->print(getTemperature()) + se->print(";") +
        se->print(getCurrent()) + se->print(";") +
        se->print(getVoltage());
    se->flush();
    return size;
}

V2LibsSlave::V2LibsSlave() : SOFT_COMM(10,11) {
    pcCommEnabled = false;
    softCommEnabled = false;
    tempSensorEnabled = false;
    currentSensorEnabled = false;
    voltageSensorEnabled = false;
}

V2LibsSlave::~V2LibsSlave() {
}

void V2LibsSlave::maybeDebug(const char *s) {
    //PC_COMM.println(s);
    //PC_COMM.flush();
}

void V2LibsSlave::setupPcComm() {
    PC_COMM.begin(9600); // Serial  @ 57600
    while(!PC_COMM);

    maybeDebug("PC connected OK!");
    pcCommEnabled = true;
}
void V2LibsSlave::setupSoftComm() {
    SOFT_COMM.begin(9600);  // Serial3 @ 57600
    //while(!SOFT_COMM);
    maybeDebug("Software Serial connected OK!");
    softCommEnabled = true;
}
void V2LibsSlave::setupV2LibsComm() {
    v2libsComm.setup(&PC_COMM, &SOFT_COMM);
}
void V2LibsSlave::setupId() {
    eepromId = eepromWriter.readId();
}
void V2LibsSlave::setupTempSensor() {
    if (!tempSensor.setup()) {
        maybeDebug("Couldn't find MCP9808!");
        //while (1);
    } else {
        maybeDebug("MCP9808 setup OK!");
        tempSensorEnabled = true;
    }
}
void V2LibsSlave::setupCurrentSensor() {
    currentSensor.setup(A0);
    maybeDebug("Current sensor OK!");
    currentSensorEnabled = true;
}
void V2LibsSlave::setupVoltageSensor() {
    voltageSensor.setup(A1);
    maybeDebug("Voltage divider OK!");
    voltageSensorEnabled = true;
}

float V2LibsSlave::getTemperature() {
    return tempSensor.readCelsius();
}

float V2LibsSlave::getCurrent() {
    return toAmps(toVolts(currentSensor.read()));
}

float V2LibsSlave::getVoltage() {
  return (toVolts(voltageSensor.read()) * R1_R2) / R2;
}

void V2LibsSlave::setup() {
    pinMode(13, OUTPUT);
    setupPcComm();
    setupSoftComm();
    setupV2LibsComm();
    setupId();
    //setupTempSensor();
    setupCurrentSensor();
    setupVoltageSensor();
}
static unsigned int last_t = 0;
void V2LibsSlave::loop() {
    v2libsComm.loop();

    static int VALUE = HIGH;

    int code = v2libsComm.getCommand();
    if (code != CMD_INVALID) {
        VALUE = (VALUE == HIGH ? LOW : HIGH);
        digitalWrite(13, VALUE);

        v2libsComm.takeCommand();
        switch(code) {
        case CMD_TEMPERATURE:
            PC_COMM.print('<');
            PC_COMM.print(eepromId);
            PC_COMM.print(';');
            PC_COMM.print(getTemperature());
            PC_COMM.println('>');
            SOFT_COMM.println('T');
            break;
        case CMD_CURRENT:
            PC_COMM.print('<');
            PC_COMM.print(eepromId);
            PC_COMM.print(';');
            PC_COMM.print(getCurrent());
            PC_COMM.println('>');
            SOFT_COMM.println('C');
            break;
        case CMD_VOLTAGE:
            PC_COMM.print('<');
            PC_COMM.print(eepromId);
            PC_COMM.print(';');
            PC_COMM.print(getVoltage());
            PC_COMM.println('>');
            SOFT_COMM.println('V');
            break;
        case CMD_ANALOG:
            PC_COMM.print('<');
            PC_COMM.print(eepromId);
            PC_COMM.print(';');
            sendAnalogValues(&PC_COMM);
            PC_COMM.println('>');
            SOFT_COMM.println('S');
            break;
        default:
            PC_COMM.println("<Commando invalido>");
            break;
        }

    } // code != CMD_NONE

}


