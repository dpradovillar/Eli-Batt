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

volatile long TOGGLE_COUNT = 0;

ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}

ISR(INT0_vect) {
}

SerialEndpoint pcComm;
RtcClock rtcClock;

void setup()
{
    pcComm.setup(0, 1, 9600);
    pcComm.waitForConnection();
    pcComm.println("connection to pc stablished");

    Wire.begin();
    
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

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    Serial.print("Temp as float: ");
    Serial.print(temp_float, DEC);
    Serial.println();
    Serial.print("Temp as word: ");
    Serial.print(temp_hbyte, DEC);
    Serial.print(".");
    Serial.print(temp_lbyte, DEC);
    Serial.println();

    Serial.println();

    delay(1000);
}

