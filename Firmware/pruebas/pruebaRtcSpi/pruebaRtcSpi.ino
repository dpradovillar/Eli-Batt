#include <elibatt_config.h>

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>

#include <RtcInput.h>
#include <Adafruit_GPS.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Utils.h>

RtcClock rtc;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  rtc.setup(34);
  
  MyDate d;
  d.year = 15;
  d.month = 6;
  d.day = 23;
  
  d.hour = 16;
  d.minute = 9;
  d.second = 10;
  
  rtc.adjust(d);
}

void loop() {
  MyDate d = rtc.readDate();
  
  Serial.print(d.year); Serial.print("/");
  Serial.print(d.month); Serial.print("/");
  Serial.print(d.day); Serial.print(" ");
    
  Serial.print(d.hour); Serial.print(":");
  Serial.print(d.minute); Serial.print(":");
  Serial.print(d.second); Serial.println();
  
  delay(1000);
}


