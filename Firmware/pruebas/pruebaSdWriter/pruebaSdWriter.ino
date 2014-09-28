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
SdWriter sdWriter;

class MyCallback : public SampleCallback {
public:
  virtual ~MyCallback(){}
  virtual void eventDetected(uint32_t current_usecs) {
    
    debugSerialEndpoint.println("Trying to open file");
    if (sdWriter.open()) {
      debugSerialEndpoint.println("file open ok!");
      
      char buffer[8];
      Utils::toHex(buffer, current_usecs);
      sdWriter.write(buffer, 8);
      sdWriter.write("\r\n", 2);
      if (sdWriter.close()) {
        debugSerialEndpoint.println("file closed ok!");
      } else {
        debugSerialEndpoint.println("couldn't close file, maybe it was unopened?");
      }
    }else{
      debugSerialEndpoint.println("couldn't open file!");
    }
  }
};

//LedBlinkCallback ledBlinkCallback;
MyCallback myCallback;

void setup() {
  debugSerialEndpoint.setup(0, 1, 9600);
  debugSerialEndpoint.waitForConnection();
  
//  ledBlinkCallback.setup(7);
  
  debugSerialEndpoint.println("trying to start SD card");
  if(!sdWriter.setup(4, &debugSerialEndpoint)) { // chipSelect on pin 4, additionally, pin 10 is left as OUTPUT
    debugSerialEndpoint.println("couldn't set it up");
    while(1);
  } 
  
  uint32_t id = eepromWriter.readId();
  debugSerialEndpoint.print("Id from eeprom:");
  debugSerialEndpoint.println(id);
  
  sampleClock.setup(0, &myCallback);
  //sampleClock.setup(1, &ledBlinkCallback);
  sampleClock.setPeriod(5*1000000); // every 5 seconds
}

void loop() {
  sampleClock.loop();
}
