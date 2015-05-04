#include <elibatt_config.h>

#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <I2cInput.h>
#include <RTC_DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>
#include <RtcInput.h>

// ------ Begin of required section for RtcInput ------
volatile long TOGGLE_COUNT = 0;
ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}
ISR(INT0_vect) {
}
// ------- End of required section for RtcInput -------

SerialEndpoint pcComm;

Mcp9808Sensor tempSensor;
RtcClock rtcClock;

void setup() {  
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();
  
  delay(500);
  
  pcComm.println("PC serial connection OK!");
  
  // Setup other stuff
  if(tempSensor.setup()) { // Uses A4 and A5.
    pcComm.println("MCP9808 started OK!");
  } else {
    pcComm.println("MCP9808 couldn't start!");
    while(1);
  }
  
  if (rtcClock.setup(&pcComm)) {
    pcComm.println("RTC is OK!");
  } else {
    pcComm.println("RTC is in bad shape!");
    while(1);
  }
}

void loop() {
  float temp_mcp9808 = tempSensor.readCelsius();
  float temp_rtc = rtcClock.getTempAsFloat();

  Serial.print("MCP9808: "); Serial.println(temp_mcp9808);
  Serial.print("RTC    : "); Serial.println(temp_rtc);
  Serial.println();

  delay(1000);
}
