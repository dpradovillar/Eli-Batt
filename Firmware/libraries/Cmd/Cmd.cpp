#include <Cmd.h>

Cmd::Cmd() {
}

Cmd::~Cmd() {
}

void Cmd::setup(SerialEndpoint *endpoint) {
    out = endpoint;
}

bool Cmd::equals(char *s, int len_s, char *t, int len_t){
    if (len_s != len_t){
        return false;
    }
    for(int i=0; i<len_s; i++) {
        if (s[i] != t[i]){
            return false;
        }
    }
    return true;
}

bool Cmd::startsWith(char *s, int len_s, char *t, int len_t){
    if(len_t > len_s){
        return false;
    }
    return equals(s, len_t, t, len_t);
}

int Cmd::parseCmd(char *s, int len) {
    // GPS
    if(startsWith(s, len, "GPS", 3)){
        return CMD_GPS;
    }
    if(startsWith(s, len, "LA", 2)){
        return CMD_LATITUDE;
    }
    if(startsWith(s, len, "LO", 2)){
        return CMD_LONGITUDE;
    }
    if(startsWith(s, len, "H", 1)){
        return CMD_HEIGHT;
    }

    // Average
    if(startsWith(s, len, "TX", 2)) {
        return CMD_AVG_TEMPERATURE;
    }
    if(startsWith(s, len, "CX", 2)) {
        return CMD_AVG_CURRENT;
    }
    if(startsWith(s, len, "VX", 2)) {
        return CMD_AVG_VOLTAGE;
    }

    // Lists
    if(startsWith(s, len, "TS", 2)) {
        return CMD_LIST_TEMPERATURE;
    }
    if(startsWith(s, len, "CS", 2)) {
        return CMD_LIST_CURRENT;
    }
    if(startsWith(s, len, "VS", 2)) {
        return CMD_LIST_VOLTAGE;
    }

    // T,C,V + S
    if(startsWith(s, len, "T", 1)){
        return CMD_TEMPERATURE;
    }
    if(startsWith(s, len, "C", 1)){
        return CMD_CURRENT;
    }
    if(startsWith(s, len, "V", 1)){
        return CMD_VOLTAGE;
    }
    if(startsWith(s, len, "S", 1)) {
        return CMD_SENSORS;
    }

    // Other
    if(startsWith(s, len, "N", 1)) {
        return CMD_LIST_SIZE;
    }
    if(startsWith(s, len, "R0", 2)) {
        return CMD_RELAY_OFF;
    }
    if(startsWith(s, len, "R1", 2)) {
        return CMD_RELAY_ON;
    }
    if(startsWith(s, len, "R?", 2)) {
        return CMD_RELAY_STATUS;
    }

    // IDS,ID
    if(startsWith(s, len, "IDS", 3)) {
        return CMD_LIST_ID;
    }
    if(startsWith(s, len, "ID", 2)) {
        return CMD_ID;
    }

    return CMD_INVALID;
}

