#include <elibatt_config.h>

#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <I2cInput.h>
#include <MemoryFree.h>
#include <OneWire.h>
#include <OneWireInput.h>
#include <RTClib.h>
#include <RtcInput.h>
#include <SD.h>
#include <SdData.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

// ------ Begin of required section for RtcInput ------
volatile long TOGGLE_COUNT = 0;
ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}
ISR(INT0_vect) {
}
// ------- End of required section for RtcInput -------

SerialEndpoint pcComm;

//Mcp9808Sensor tempSensor;
Ds1820Sensor tempSensor;
RtcClock rtcClock;

int fileCount = 0;

const int mosiOneWirePin = 51;
const int sdChipSelectPin = 38;

void setup() {  
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();
  
  delay(500);
  
  pcComm.println("PC serial connection OK!");
  
  // Setup other stuff
  if(tempSensor.setup(mosiOneWirePin)) {
    pcComm.println("DS1820 started OK!");
  } else {
    pcComm.println("DS1820 couldn't start!");
    while(1);
  }
  
  rtcClock.setup();
  
  pinMode(mosiOneWirePin, OUTPUT);
  
  if (!SD.begin(mosiOneWirePin)) {
    pcComm.println("Couldnt initialized SD on pin 38");
    while(1);
  }
  
  // Setup SdWriter
  /*
  if(sdWriter.setup(38, &pcComm)) {
    pcComm.println("SD set up OK!");
  } else {
    pcComm.println("SD in bad shape! :(");
    while(1);
  }*/
}

void loop() {

    Serial.print("Free Memory:"); Serial.println(freeMemory());
    Serial.flush();

  float temp_mcp9808 = tempSensor.readCelsius();
  float temp_rtc = 0;//rtcClock.getTempAsFloat();
  


/*
  sdWriter.open();

  sdWriter.write("MCP9808: ", 9); sdWriter.writeFloat(temp_mcp9808); sdWriter.writeNewline();
  sdWriter.write("RTC    : ", 9); sdWriter.writeFloat(temp_rtc);     sdWriter.writeNewline();
  sdWriter.writeNewline();
*/
char filename[] = "test?.csv";
filename[4] = 'A'+fileCount;

Serial.print("Wirting to"); Serial.println(filename);

File f = SD.open(filename, FILE_WRITE);
if (f) {
  f.print("MCP9808: "); f.print(temp_mcp9808); f.println();
  f.print("RTC    : "); f.print(temp_rtc);     f.println();
  f.println();
  f.close();
}

  pcComm.print("MCP9808: "); pcComm.print(temp_mcp9808); pcComm.println();
  pcComm.print("RTC    : "); pcComm.print(temp_rtc);     pcComm.println();
  pcComm.println();

fileCount++;

  delay(1000);
}
