#include <elibatt_config.h>

#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Adafruit_GPS.h>
#include <I2cInput.h>
#include <SPI.h>
#include <AdafruitSD.h>
#include <Cmd.h>
#include <OneWireInput.h>
#include <AnalogInput.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <EepromWriter.h>
#include <Wire.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <ParserComm.h>
#include <RtcInput.h>
#include <SdData.h>
#include <MemoryFree.h>
#include <OneWire.h>
#include <SimpleCrc.h>
#include <OneWireInput.h>

Ds1820Sensor sensor;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  sensor.setup(32);
}

void loop() {
  static int last_t = 0;
  
  if (millis() - last_t > 2000) {
    
    float c = sensor.readCelsius();
    Serial.print("Temp=");
    Serial.println(c);
    
    last_t = millis();
  }
}


