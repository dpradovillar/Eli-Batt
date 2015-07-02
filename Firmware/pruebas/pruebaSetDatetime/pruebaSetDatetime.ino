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
  Serial.begin(57600);

  SPI.begin();

  rtc.setup(34);
  
  int timeout = 5000;
  uint32_t last_time = millis();
  
  // If didnt recevie anything in some seconds, skip adjusting of time
  while(Serial.available() < 6) {
    if ((millis() - last_time) >= timeout) {
      return;
    }
  }
  
  MyDate d;
  d.year = Serial.read();
  d.month = Serial.read();
  d.day = Serial.read();
  
  d.hour = Serial.read();
  d.minute = Serial.read();
  d.second = Serial.read();
  
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


