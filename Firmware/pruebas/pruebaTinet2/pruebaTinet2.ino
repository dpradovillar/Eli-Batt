#include <elibatt_config.h>

// Note: these includes are necessary for the Arduino IDE to know which libraries to pre-compile.
#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <RTC_DS3231.h>
#include <RtcInput.h>
#include <SD.h>
#include <SdData.h>
#include <SPI.h>
#include <UnixCmd.h>
#include <Utils.h>
#include <V2Libs.h>
#include <Wire.h>

V2Libs v2libs;

void setup() {
    v2libs.setup();
}

void loop() {
    v2libs.loop();
}
