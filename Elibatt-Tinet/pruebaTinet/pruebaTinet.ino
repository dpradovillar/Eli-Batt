// Sensor de Temperatura
#include <Wire.h>
#include "Adafruit_MCP9808.h"
// BLEMini
#include <Arduino.h>
#include <AltSoftSerial.h>
// Sensor de Corriente y Voltaje
#include <AnalogInput.h>
// GPS
#include <Adafruit_GPS.h>
#include <ArduinoSoftwareSerial.h>
#include <Endpoint.h>
#include <GpsInput.h>
#include <Utils.h>
// SD
#include <SD.h>


// Crear objeto del sensor de temperatura MCP9808
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

// BLEMini
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328P__)  
  AltSoftSerial BLEMini;  
#else
  #define BLEMini Serial3
#endif

// Crear objeto de sensor de Corriente y Voltaje
AnalogInput currentSensor;
AnalogInput voltageSensor;
  // Se definen valores para los sensores de corriente y voltaje
#define MAX_BATT_V 12.6
#define MAX_VOLTS 3.31
#define R1_R2 12940.0
#define R2 3210.0

// Crear objeto del GPS
GpsInput g;
SerialEndpoint pcComm;

// Para SD
File myFile;
#define FILENAME_MAX 32
#define BUFFER_MAX   100
#define MAX_ROWS     10
int countRows = 0;


void Temperature(bool removeIt=false) {
  float c = tempsensor.readTempC();   // Grados Celcius
  float f = c * 9.0 / 5.0 + 32;       // Grados Fahrenheit
  BLEMini.println(c);
}

void Current(bool removeIt=false) {
  float vcurrent = toVolts(currentSensor.read());
  float current = toAmps(vcurrent);
  BLEMini.println(current);
}

void Voltage(bool removeIt=false) {
  float voltage = toVolts(voltageSensor.read());
  float realv = (voltage * R1_R2) / R2;  
  BLEMini.println(realv);
}

float toAmps(float vout3v) {
  //float vout12v = to12v(vout3v);
  return (36.7f * vout3v) / MAX_VOLTS - 18.3f;
}

float toVolts(int reading) {
  return (MAX_VOLTS * reading) / 1023;
}

void GPS(bool removeIt=false) {
  if (g.available()) {
      GpsStruct gdata = g.getData();
      if (gdata.fix) {
        BLEMini.print("{'GPS': [ { 'Fix' : '");
        BLEMini.print("1");
        BLEMini.print("', 'Latitud' : '");
        BLEMini.print(gdata.int_lat); BLEMini.print("."); BLEMini.print(gdata.dec_lat);
        BLEMini.print("', 'Longitud' : '");
        BLEMini.print(gdata.int_lon); BLEMini.print("."); BLEMini.print(gdata.dec_lon);
        BLEMini.print("', 'Altura' : '");
        BLEMini.print(gdata.int_altitude); BLEMini.print("."); BLEMini.print(gdata.dec_altitude);
        BLEMini.println("' } ] }");
      }
      else
      {
        BLEMini.print("{'GPS': [ { 'Fix' : '");
        BLEMini.print("0");
        BLEMini.print("', 'Latitud' : '");
        BLEMini.print(gdata.int_lat); BLEMini.print("."); BLEMini.print(gdata.dec_lat);
        BLEMini.print("', 'Longitud' : '");
        BLEMini.print(gdata.int_lon); BLEMini.print("."); BLEMini.print(gdata.dec_lon);
        BLEMini.print("', 'Altura' : '");
        BLEMini.print(gdata.int_altitude); BLEMini.print("."); BLEMini.print(gdata.dec_altitude);
        BLEMini.println("' } ] }");
      }
  }
  
}

void Latitud(bool removeIt=false) {
  if (g.available()) {
      GpsStruct gdata = g.getData();
      if (gdata.fix) {
        BLEMini.print(gdata.int_lat); BLEMini.print("."); BLEMini.println(gdata.dec_lat);
      }
      else
      {
        BLEMini.println("-1");
      }
  }
}

void Longitud(bool removeIt=false) {
  if (g.available()) {
      GpsStruct gdata = g.getData();
      if (gdata.fix) {
        BLEMini.print(gdata.int_lon); BLEMini.print("."); BLEMini.println(gdata.dec_lon);
      }
      else
      {
        BLEMini.println("-1");
      }
  }
}

void Altura(bool removeIt=false) {
  if (g.available()) {
      GpsStruct gdata = g.getData();
      if (gdata.fix) {
        BLEMini.print(gdata.int_altitude); BLEMini.print("."); BLEMini.println(gdata.dec_altitude);
      }
      else
      {
        BLEMini.println("-1");
      }
  }
}

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
         BLEMini.print("Removing (");
         BLEMini.print(entry.name());
         BLEMini.print("): ");
       }
    } else {
      if (entry.isDirectory()) {
        BLEMini.print("D ");
      }else{
        BLEMini.print(entry.size());
        BLEMini.print(' ');
      }
      BLEMini.print(entry.name());
      BLEMini.println();
    }

    // If remove it, remove it
    if (removeIt) {
      BLEMini.println(SD.remove(entry.name()) ? "OK" : "ERROR");
    }
    
    entry.close();
  }
  root.close();
}

void mv(char *filename1, char *filename2) {
  File from = SD.open(filename1);
  if (!from){
    BLEMini.print("Can't open file ");
    BLEMini.println(filename1);
    return;
  }
  
  File to = SD.open(filename2, FILE_WRITE);
  if(!to){
    BLEMini.print("Can't create file");
    BLEMini.println(filename2);
    
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
  BLEMini.println("File renamed OK!");
}

void rm(char *filename) {
  
  if (equals(filename, 1, "*", 1)) {
    BLEMini.println("Removing everything!");
    ls(1);
    return;
  }  
  File dataFile = SD.open(filename);
  if (dataFile) {
    dataFile.close();
    BLEMini.println(SD.remove(filename) ? "File removed OK!" : "File couldn't be removed");
  } else {
    BLEMini.println("Can't remove a file that can't be open!");
  }
}

void cat(char *filename) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available()) {
      BLEMini.write(dataFile.read());
      BLEMini.flush();
    }
    dataFile.close();
  } else {
    BLEMini.println("File is not available, maybe it doesn't exist?");
  }
}

void touch(char *filename) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.close();
    BLEMini.println("File created OK!");
  } else {
    BLEMini.println("Can't create that file!");
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
  
  if(startsWith(s, len, "T", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return 5;
  }
  
  if(startsWith(s, len, "C", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return 6;
  }
  
  if(startsWith(s, len, "V", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return 7;
  }
  
  if(startsWith(s, len, "GPS", 3)){
    f1[0] = 0;
    f2[0] = 0;
    return 8;
  }
  
  if(startsWith(s, len, "LA", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return 9;
  }
  
  if(startsWith(s, len, "LO", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return 10;
  }
  
  if(startsWith(s, len, "H", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return 11;
  }
  
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

void setup() {
  Serial.begin(57600);
  BLEMini.begin(57600);
  
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1); 
  }
  
  // Setup other stuff
  currentSensor.setup(A0);
  voltageSensor.setup(A1);
  
  // Setup GPS
  pcComm.setup(0, 1, 57600);
  pcComm.waitForConnection();
  g.setup(&Serial2, 9600, &pcComm);
  
  // Setup SD
  Serial.print("Initializing SD card...");
  if (!SD.begin(38)) {
      Serial.println("Error conexión SD.");
  }
  Serial.println("initialization done.");
  
}

long last_t = 0;  // Para loop()

void loop() {

  g.loop();  // GPS
  
  static char cmdBuffer[BUFFER_MAX];
  static int cmdLen = 0;
  static char filename1[FILENAME_MAX+1];
  static char filename2[FILENAME_MAX+1];
  
  if (BLEMini.available()) {
    char c = BLEMini.read();
    cmdBuffer[cmdLen++] = c;
    
    if (cmdLen >= BUFFER_MAX || cmdBuffer[cmdLen-1] == '\n') {
        switch(parseCmd(cmdBuffer, cmdLen, filename1, filename2)){
          case 0:
            ls();
            break;
          case 1:
            mv(filename1, filename2);
            break;
          case 2:
            rm(filename1);
            break;
          case 3:
            cat(filename1);
            break;
          case 4:
            touch(filename1);
            break;
          case 5:
            Temperature();
            break;
          case 6:
            Current();
            break;
          case 7:
            Voltage();
            break;
          case 8:
            GPS();
            break;
          case 9:
            Latitud();
            break;
          case 10:
            Longitud();
            break;
          case 11:
            Altura();
            break;
          default:
            BLEMini.println("Command not found!");
            BLEMini.println("############################");
            break;
        }
        cmdLen = 0;
    }
    
  }
  
  // ########################## Sensors Handling ####################
  if (millis() - last_t >= 2000) {    
    // Read and print out the temperature, then convert to *F
    float c = tempsensor.readTempC();
    float f = c * 9.0 / 5.0 + 32;
    Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t"); 
    Serial.print(f); Serial.println("*F");
    
    // Leer sensores de corriente y voltaje
    float vcurrent = toVolts(currentSensor.read());
    float current = toAmps(vcurrent);
    Serial.print("Corriente: "); Serial.print(current); Serial.println("[A]");
    
    float voltage = toVolts(voltageSensor.read());
    float realv = (voltage * R1_R2) / R2;
    Serial.print("Voltaje: "); Serial.print(realv); Serial.println("[V]");
    
    // Valores GPS
    float latitud = 0;
    float longitud = 0;
    float altura = 0;
    
    // ########################## GPS Handling ####################  
    if (g.available()) {
      GpsStruct gdata = g.getData();
      pcComm.print("Time: ");
      pcComm.print((int)gdata.hour); pcComm.print(':');
      pcComm.print((int)gdata.minute); pcComm.print(':');
      pcComm.print((int)gdata.second); pcComm.println();
      
      pcComm.print("Date: ");
      pcComm.print((int)gdata.day); pcComm.print('/');
      pcComm.print((int)gdata.month); pcComm.print("/");
      pcComm.println((int)gdata.year);
      
      pcComm.print("Fix: "); pcComm.print((int)gdata.fix);
      pcComm.print(" quality: "); pcComm.println((int)gdata.quality);
      
      if (gdata.fix) {
        String str1;
        str1 += String(gdata.int_lat);
        str1 += ".";
        str1 += String(gdata.dec_lat);
        char floatbuf[11]; // make this at least big enough for the whole string
        str1.toCharArray(floatbuf, sizeof(floatbuf));
        latitud = atof(floatbuf);
        
        str1 = "";
        str1 += String(gdata.int_lon);
        str1 += ".";
        str1 += String(gdata.dec_lon);
        char floatbuf2[11]; // make this at least big enough for the whole string
        str1.toCharArray(floatbuf2, sizeof(floatbuf2));
        longitud = atof(floatbuf2);
        
        str1 = "";
        str1 += String(gdata.int_altitude);
        str1 += ".";
        str1 += String(gdata.dec_altitude);
        char floatbuf3[11]; // make this at least big enough for the whole string
        str1.toCharArray(floatbuf3, sizeof(floatbuf3));
        altura = atof(floatbuf3);
                        
        pcComm.print("Location: ");
        pcComm.print(gdata.int_lat); pcComm.print("."); pcComm.print(gdata.dec_lat);
        pcComm.print(", "); 
        pcComm.print(gdata.int_lon); Serial.print("."); pcComm.print(gdata.dec_lon);
        
        pcComm.print("Speed (knots): "); pcComm.print(gdata.int_speed);    pcComm.print(".");  pcComm.println(gdata.dec_speed);
        pcComm.print("Angle: ");         pcComm.print(gdata.int_angle);    pcComm.print(".");  pcComm.println(gdata.dec_angle);
        pcComm.print("Altitude: ");      pcComm.print(gdata.int_altitude); pcComm.print(".");  pcComm.println(gdata.dec_altitude);
        pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
      }
    }
     
    // Inicializar archivo Tarjeta Sd
    if (countRows == 0) {
      Serial.print("Initializing test...txt");
      char filename[] = "test...txt";
      filename[4] = ('0' + (countRows / 10) % 10);
      filename[5] = ('0' + countRows % 10);
      myFile.close();
      myFile = SD.open(filename, FILE_WRITE);
      if (!myFile) {
        // if the file didn't open, print an error:
        Serial.println("Error opening test...txt");
      }
      else
      {
        Serial.println("Abriendo test...txt:");
      }
    }
    
    // ###### Grabar en la SD ########
    // -> VOLTAGE, CURRENT, TEMPERATURE, LATITUD, LONGITUD, ALTURA  ######
      
    // Voltage 3.3v --> to 12v
    //myFile.print(voltage); myFile.print(";");
    myFile.print(realv);     myFile.print(";");
    
    // Voltage 3.3v --> to amps
    //myFile.print(vcurrent);myFile.print(";");
    myFile.print(current);   myFile.print(";");
    
    // Temperatura
    myFile.print(c);         myFile.print(";");
    
    // Latitud
    myFile.print(latitud);   myFile.print(";");

    // Longitud
    myFile.print(longitud);  myFile.print(";");
     
    // Altura
    myFile.print(altura);    myFile.print(";");
     
    myFile.println();
  
    countRows++;
    if (countRows == MAX_ROWS) {
      countRows = 0;
    }
    
    // #### Enviar información a Dispositivo Movil en tiempo real ####
    // #### VOLTAGE, CURRENT, TEMPERATURE #####
      
//    // Voltage 3.3v --> to 12v
//    //Serial.print(voltage); Serial.print(";");
//    BLEMini.print(realv);    BLEMini.print(";");
//  
//    // Voltage 3.3v --> to amps
//    //Serial.print(vcurrent);Serial.print(";");
//    BLEMini.print(current);  BLEMini.print(";");
//    
//    // Temperatura 
//    BLEMini.print(c);        BLEMini.print(";");
//    
//    // Latitud
//    BLEMini.print(latitud);  BLEMini.print(";");
//
//    // Longitud
//    BLEMini.print(longitud); BLEMini.print(";");
//     
//    // Altura
//    BLEMini.print(altura);   BLEMini.print(";");
//    
//    BLEMini.println();
    
    Serial.println("############################");
    
    last_t = millis();
  }

}
