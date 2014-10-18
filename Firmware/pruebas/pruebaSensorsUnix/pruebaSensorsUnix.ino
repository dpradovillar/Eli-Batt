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

#define FILENAME_MAX 32
#define BUFFER_MAX   100

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

const int chipSelect = 38;

// Use Serial3 when using BLE
// Use Serial when using PC
#define SERIAL Serial3

void ls(bool removeIt=false) {
  File root = SD.open("/");
  root.rewindDirectory();
  while(true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    if (removeIt) {
       if (!entry.isDirectory()) {
         SERIAL.print("Removing (");
         SERIAL.print(entry.name());
         SERIAL.print("): ");
       }
    } else {
      if (entry.isDirectory()) {
        SERIAL.print("D ");
      }else{
        SERIAL.print(entry.size());
        SERIAL.print(' ');
      }
      SERIAL.print(entry.name());
      SERIAL.println();
    }

    // If remove it, remove it
    if (removeIt) {
      SERIAL.println(SD.remove(entry.name()) ? "OK" : "ERROR");
    }
    
    entry.close();
  }
  root.close();
}

void mv(char *filename1, char *filename2) {
  File from = SD.open(filename1);
  if (!from){
    SERIAL.print("Can't open file ");
    SERIAL.println(filename1);
    return;
  }
  
  File to = SD.open(filename2, FILE_WRITE);
  if(!to){
    SERIAL.print("Can't create file");
    SERIAL.println(filename2);
    
    from.close();
    return;
  }
  
  while(from.available()){
    char c = from.read();
    to.write(c);
  }
  from.close();
  to.close();
  SD.remove(filename1);
  SERIAL.println("File renamed OK!");
}

void rm(char *filename) {
  
  if (equals(filename, 1, "*", 1)) {
    SERIAL.println("Removing everything!");
    ls(1);
    return;
  }  
  File dataFile = SD.open(filename);
  if (dataFile) {
    dataFile.close();
    SERIAL.println(SD.remove(filename) ? "File removed OK!" : "File couldn't be removed");
  } else {
    SERIAL.println("Can't remove a file that can't be open!");
  }
}

void cat(char *filename) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available()) {
      SERIAL.write(dataFile.read());
      SERIAL.flush();
    }
    dataFile.close();
  } else {
    SERIAL.println("File is not available, maybe it doesn't exist?");
  }
}

void touch(char *filename) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.close();
    SERIAL.println("File created OK!");
  } else {
    SERIAL.println("Can't create that file!");
  }
}

int readNextToken(char *s, int len, char *t, int &r, int max_t_len) {
  r = 0;
  int i = 0;
  while(i<len && s[i] <= ' ') i++;
  
  if (i>=len) {
    t[0] = 0;
    return -1;
  }

  int j = 0;
  while(i<len && s[i] > ' ' && j < max_t_len) {
    t[j++] = s[i];
    i++;
  }
  r = j;
  return i;
}

bool equals(char *s, int len_s, char *t, int len_t){
  if (len_s != len_t){
    return false;
  }
  for(int i=0; i<len_s; i++) {
    if (s[i] != t[i]){
      return false;
    }
  }
  return true;
}

bool startsWith(char *s, int len_s, char *t, int len_t){
  if(len_t > len_s){
    return false;
  }
  return equals(s, len_t, t, len_t);
}

int parseCmd(char *s, int len, char *f1, char *f2) {
  int ts, tl;
  
  if(startsWith(s, len, "ls", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return 0;
  }
  
  if(startsWith(s, len, "mv ", 3)){
    s += 3;
    len -= 3;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    s += skipped;
    len -= skipped;
    
    skipped = readNextToken(s, len, f2, r, FILENAME_MAX);
    if (r < 0) return -1;
    f2[r] = 0;

    return 1;
  }
  
  if (startsWith(s, len, "rm ", 3)){
    s += 3;
    len -= 3;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 2;
  }
  
  if (startsWith(s, len, "cat ", 4)){
    s += 4;
    len -= 4;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 3;
  }
  
  if (startsWith(s, len, "touch ", 6)){
    s += 6;
    len -= 6;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 4;
  }
}

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

long last_t = 0;

void loop() {
  

  // ###################### SD Handling ###################################
  static char cmdBuffer[BUFFER_MAX];
  static int cmdLen = 0;
  
  static char filename1[FILENAME_MAX+1];
  static char filename2[FILENAME_MAX+1];
  
  if (SERIAL.available()) {
    char c = SERIAL.read();
    cmdBuffer[cmdLen++] = c;
    SERIAL.write(c);
    
    if (cmdLen >= BUFFER_MAX || cmdBuffer[cmdLen-1] == '\n') {
        switch(parseCmd(cmdBuffer, cmdLen, filename1, filename2)){
          case 0:
            ls();
            SERIAL.println("##############################");
            break;
          case 1:
            mv(filename1, filename2);
            SERIAL.println("##############################");
            break;
          case 2:
            rm(filename1);
            SERIAL.println("##############################");
            break;
          case 3:
            cat(filename1);
            SERIAL.println("##############################");
            break;
          case 4:
            touch(filename1);
            SERIAL.println("##############################");
            break;
          default:
            SERIAL.println("Command not found!");
            SERIAL.println("##############################");
            break;
        }
        cmdLen = 0;
    }
  }
  
  // ########################## Sensors Handling ####################
  
  if (millis() - last_t >= 1000) {
  
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
      
      last_t = millis();
  }

}
