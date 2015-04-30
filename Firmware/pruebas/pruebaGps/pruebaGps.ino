#include <elibatt_config.h>

#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

SerialEndpoint pcComm;

GpsInput g;

void setup() {
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();

  g.setup(&Serial2, 9600);
}

uint64_t last_t = millis();
void loop() {
  g.loop();
  if (millis() - last_t > 2000) {
    if (g.available()) {
      GpsStruct gdata = g.getData();
      pcComm.printSimpleDate(gdata.day, gdata.month, gdata.year);
      pcComm.print(" ");
      pcComm.printSimpleTime(gdata.hour, gdata.minute, gdata.second);
      
      pcComm.print("  -  fix:"); pcComm.print((int)gdata.fix);
      pcComm.print(" quality:"); pcComm.println((int)gdata.quality);
      
      if (gdata.fix) {
        pcComm.print("Location: ");
        pcComm.printFloating(gdata.int_lat, gdata.dec_lat);
        pcComm.print(", "); 
        pcComm.printlnFloating(gdata.int_lon, gdata.dec_lon);
        
        pcComm.print("Speed (knots): "); pcComm.printlnFloating(gdata.int_speed, gdata.dec_speed);
        pcComm.print("Angle: ");         pcComm.printlnFloating(gdata.int_angle, gdata.dec_angle);
        pcComm.print("Altitude: ");      pcComm.printlnFloating(gdata.int_altitude, gdata.dec_altitude);
        pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
      }
    }
    last_t = millis();
  }
}
