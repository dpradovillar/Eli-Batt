#include <Adafruit_GPS.h>
#include <Adafruit_MCP9808.h>
#include <AnalogInput.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <GpsInput.h>
#include <I2cInput.h>
#include <RTC_DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>
#include <RtcInput.h>
#include <RTClib.h>
#include <SD.h>
#include <AltSoftSerial.h>

File myFile;

#define BLEMini Serial3

volatile long TOGGLE_COUNT = 0;

ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}

ISR(INT0_vect) {
}

Mcp9808Sensor tempSensor;
AnalogInput currentSensor;
AnalogInput voltageSensor;
RtcClock rtcClock;

SerialEndpoint pcComm;

#define MAX_ROWS 30
int countRows = 0;

void showError(int time) {
  while(1) {
    digitalWrite(13, LOW);
    delay(time);
    digitalWrite(13, HIGH);
    delay(time);
  }
}

void setup() {
  BLEMini.begin(57600);
  pinMode(13, OUTPUT);
  
  Wire.begin();
    
  if (!rtcClock.setup(&pcComm)) {
      showError(1000);
  }
  
  if (!SD.begin(38)) {
      showError(2000);
  }
  
  // Setup other stuff
  //tempSensor.setup(); // uses A4 and A5
  currentSensor.setup(A0);
  voltageSensor.setup(A1);
}

#define MAX_BATT_V 12.6
#define MAX_VOLTS 3.31

#define R1_R2 12940.0
#define R2 3210.0

float toVolts(int reading) {
  return (MAX_VOLTS * reading) / 1023;
}

float toAmps(float vout3v) {
  //float vout12v = to12v(vout3v);
  return (36.7f * vout3v) / MAX_VOLTS - 18.3f;
}

float to12v(float lowv) {
  return (lowv / MAX_VOLTS * MAX_BATT_V) ;
}

void loop() {
  float temp = tempSensor.readCelsius();
  float vcurrent = toVolts(currentSensor.read());
  float current = toAmps(vcurrent);
  float voltage = toVolts(voltageSensor.read());
  
  float realv = (voltage * R1_R2) / R2;
  float temp_float = rtcClock.getTempAsFloat();

  // VOLTAGE, CURRENT, TEMPERATURE
  
  // Voltage 3.3v --> to 12v
  //Serial.print(voltage); Serial.print(";");
  BLEMini.print(realv);   BLEMini.print(";");

  // Voltage 3.3v --> to amps
  //Serial.print(vcurrent);Serial.print(";");
  BLEMini.print(current); BLEMini.print(";");
  
  BLEMini.print(temp_float);BLEMini.print(";");
  
  BLEMini.println();
  
  if (countRows == 0) {
    char filename[] = "test...txt";
    filename[4] = ('0' + (countRows / 10) % 10);
    filename[5] = ('0' + countRows % 10);
    myFile.close();
    myFile = SD.open(filename, FILE_WRITE);
    if (!myFile) {
      showError(5000);
    }
  }
  
  // VOLTAGE, CURRENT, TEMPERATURE
  
  // Voltage 3.3v --> to 12v
  //myFile.print(voltage); myFile.print(";");
  myFile.print(realv);   myFile.print(";");
  
  // Voltage 3.3v --> to amps
  //myFile.print(vcurrent);myFile.print(";");
  myFile.print(current); myFile.print(";");
  
  myFile.print(temp_float);    myFile.print(";");
   
  myFile.println();

  countRows++;
  if (countRows == MAX_ROWS) {
    countRows = 0;
  }

  delay(1000);
}
