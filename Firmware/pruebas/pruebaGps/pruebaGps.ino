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

  g.setup(&Serial2, 9600, &pcComm);
}

uint64_t last_t = millis();
void loop() {
  g.pull();
  if (millis() - last_t > 2000) {
    if (g.available()) {
      GpsStruct GPS = g.getData();
      pcComm.print("Time: ");
      pcComm.print((int)GPS.hour); pcComm.print(':');
      pcComm.print((int)GPS.minute); pcComm.print(':');
      pcComm.print((int)GPS.second); pcComm.println();
      
      pcComm.print("Date: ");
      pcComm.print((int)GPS.day); pcComm.print('/');
      pcComm.print((int)GPS.month); pcComm.print("/");
      pcComm.println((int)GPS.year);
      
      pcComm.print("Fix: "); pcComm.print((int)GPS.fix);
      pcComm.print(" quality: "); pcComm.println((int)GPS.quality);
      
      if (GPS.fix) {
        pcComm.print("Location: ");
        pcComm.print(GPS.int_lat); pcComm.print("."); pcComm.print(GPS.dec_lat);
        pcComm.print(", "); 
        pcComm.print(GPS.int_lon); Serial.print("."); pcComm.print(GPS.dec_lon);
        
        pcComm.print("Speed (knots): "); pcComm.print(GPS.int_speed);    pcComm.print(".");  pcComm.println(GPS.dec_speed);
        pcComm.print("Angle: ");         pcComm.print(GPS.int_angle);    pcComm.print(".");  pcComm.println(GPS.dec_angle);
        pcComm.print("Altitude: ");      pcComm.print(GPS.int_altitude); pcComm.print(".");  pcComm.println(GPS.dec_altitude);
        pcComm.print("Satellites: ");    pcComm.println((int)GPS.satellites);
      }
    }
    last_t = millis();
  }
}
