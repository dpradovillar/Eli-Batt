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

#include <OneWire.h>
#include <DallasTemperature.h>

Ds1820Sensor input;

void setup(void){
  Serial.begin(9600);

  input.setup(32);
}

void printTemperature() {
  float tempC = input.readCelsius();
  Serial.print("C: ");
  Serial.print(tempC);
  Serial.println();
}

void loop(void) {   
  printTemperature();

  delay(1000);  
}

