#include <elibatt_config.h>

#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <RtcInput.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

// --------- Begins required section for RtcInput.h ---------
volatile long TOGGLE_COUNT = 0;
ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}
ISR(INT0_vect) {
}
// ---------- Ends required section for RtcInput.h ----------

SerialEndpoint pcComm;
RtcClock rtcClock;

void setup() {
    pcComm.setup(0, 1, 9600);
    pcComm.waitForConnection();
    pcComm.println("connection to pc stablished");

    delay(500);

    if (rtcClock.setup(&pcComm)) {
        pcComm.println("RTC is OK!");
    } else {
        pcComm.println("RTC is in bad shape!");
    }
}

void loop() {
    DateTime now = rtcClock.now();

    float temp_float = rtcClock.getTempAsFloat();
    int16_t temp_word = rtcClock.getTempAsWord();
    int8_t temp_hbyte = temp_word >> 8;
    int8_t temp_lbyte = temp_word &= 0x00FF;

    pcComm.printSimpleDate(now.year(), now.month(), now.day());
    pcComm.print(' ');
    pcComm.printlnSimpleTime(now.hour(), now.minute(), now.second());

    pcComm.print(" since midnight 1/1/1970 = ");
    pcComm.print(now.unixtime());
    pcComm.print("s = ");
    pcComm.print(now.unixtime() / 86400L);
    pcComm.println("d");

    pcComm.print("Temp as float: ");
    pcComm.println(temp_float);

    pcComm.print("Temp as word: ");
    pcComm.print((int)temp_hbyte);
    pcComm.print(".");
    pcComm.println((int)temp_lbyte);

    pcComm.println();

    delay(1000);
}

