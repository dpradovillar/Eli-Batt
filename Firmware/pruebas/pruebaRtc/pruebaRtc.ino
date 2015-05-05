#include <elibatt_config.h>

#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <RtcInput.h>
#include <DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <OneWire.h>
#include <Wire.h>

SerialEndpoint pcComm;
RtcClock rtcClock;

void setup() {
    pcComm.setup(0, 1, 9600);
    pcComm.waitForConnection();
    pcComm.println("connection to pc stablished");

    delay(500);
    
    Wire.begin();
    rtcClock.setup();
}

void loop() {
    int year;
    byte month, day, hour, minute, second;
    rtcClock.getTime(year, month, day, hour, minute, second);

    float temp_float = rtcClock.getTempAsFloat();

Serial.println(year);
Serial.println((int)month);
Serial.println((int)day);

Serial.println();

Serial.println((int)hour);
Serial.println((int)minute);
Serial.println((int)second);

Serial.println();

    pcComm.printSimpleDate(year, month, day);
    pcComm.print(' ');
    pcComm.printlnSimpleTime(hour, minute, second);

    pcComm.print("Temp as float: ");
    pcComm.println(temp_float);
    pcComm.println();

    delay(1000);
}

