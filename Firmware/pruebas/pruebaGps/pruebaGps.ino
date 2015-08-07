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
  g.loop();
  if (millis() - last_t > 2000) {
    if (g.available()) {
      GpsStruct gdata = g.getData();
      pcComm.print("Time: ");
      pcComm.print((int)gdata.hour); pcComm.print(':');
      pcComm.print((int)gdata.minute); pcComm.print(':');
      pcComm.print((int)gdata.second); pcComm.println();
      
      pcComm.print("Date: ");
      pcComm.print((int)gdata.day); pcComm.print('/');
      pcComm.print((int)gdata.month); pcComm.print("/");
      pcComm.println((int)gdata.year);
      
      pcComm.print("Fix: "); pcComm.print((int)gdata.fix);
      pcComm.print(" quality: "); pcComm.println((int)gdata.quality);
      
      if (gdata.fix) {
        pcComm.print("Location: ");
        pcComm.print(gdata.int_lat); pcComm.print("."); pcComm.print(gdata.dec_lat);
        pcComm.print(", "); 
        pcComm.print(gdata.int_lon); Serial.print("."); pcComm.print(gdata.dec_lon);
        
        pcComm.print("Speed (knots): "); pcComm.print(gdata.int_speed);    pcComm.print(".");  pcComm.println(gdata.dec_speed);
        pcComm.print("Angle: ");         pcComm.print(gdata.int_angle);    pcComm.print(".");  pcComm.println(gdata.dec_angle);
        pcComm.print("Altitude: ");      pcComm.print(gdata.int_altitude); pcComm.print(".");  pcComm.println(gdata.dec_altitude);
        pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
      }
    }
    last_t = millis();
  }
}
