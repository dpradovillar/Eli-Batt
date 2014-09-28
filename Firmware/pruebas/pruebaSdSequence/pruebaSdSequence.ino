#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <Debugger.h>
#include <EEPROM.h>
#include <Endpoint.h>
#include <EepromWriter.h>
#include <SampleCollector.h>
#include <SD.h>
#include <SdData.h>
#include <SimpleCrc.h>
#include <Utils.h>

SerialEndpoint debugSerialEndpoint;
EepromWriter eepromWriter;
SampleClock sampleClock;

SdNameSequencer sdNameSequencer;

class MyCallback : public SampleCallback {
public:
  virtual ~MyCallback(){}
  virtual void eventDetected(uint32_t current_usecs) {

    char buffer[13];
    sdNameSequencer.next(buffer);
    buffer[12] = 0;
    
    debugSerialEndpoint.print("next name:");
    debugSerialEndpoint.println(buffer);
  }
};

MyCallback myCallback;

void setup() {
  debugSerialEndpoint.setup(0, 1, 9600);
  debugSerialEndpoint.waitForConnection();
  
  uint32_t id = eepromWriter.readId();
  debugSerialEndpoint.print("Id from eeprom:");
  debugSerialEndpoint.println(id);
  
  sampleClock.setup(10, &myCallback);
}

void loop() {
  sampleClock.loop();
}
