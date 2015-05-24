#include <V2Libs.h>

#define OUTPUT_TO_PC true

// --------- Begins required section for RtcInput.h ---------
/*
volatile long TOGGLE_COUNT = 0;
ISR(TIMER1_COMPA_vect) {
    TOGGLE_COUNT++;
}
ISR(INT0_vect) {
}*/
// ---------- Ends required section for RtcInput.h ----------

V2Libs::V2Libs() {
    //countRows = 0;
}

V2Libs::~V2Libs() {
}

void V2Libs::sendTemperature() {
  float c = tempSensor.readCelsius(); // Grados Celcius
  float f = c * 9.0 / 5.0 + 32;       // Grados Fahrenheit
  ble.println(c);
}

void V2Libs::sendCurrent() {
  float vcurrent = toVolts(currentSensor.read());
  float current = toAmps(vcurrent);
  ble.println(current);
}

void V2Libs::sendVoltage() {
  float voltage = toVolts(voltageSensor.read());
  float realv = (voltage * R1_R2) / R2;  
  ble.println(realv);
}

float V2Libs::toAmps(float vout3v) {
  //float vout12v = to12v(vout3v);
  return (36.7f * vout3v) / MAX_VOLTS - 18.3f;
}

float V2Libs::toVolts(int reading) {
  return (MAX_VOLTS * reading) / 1023;
}

void V2Libs::sendGpsStruct() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.print("{'GPS': [ { 'Fix' : '");
        ble.print("1");
        ble.print("', 'Latitud' : '");  ble.printFloating(gdata.int_lat, gdata.dec_lat);
        ble.print("', 'Longitud' : '");  ble.printFloating(gdata.int_lon, gdata.dec_lon);
        ble.print("', 'Altura' : '");  ble.printFloating(gdata.int_altitude, gdata.dec_altitude);
        ble.println("' } ] }");
      } else {
        ble.print("{'GPS': [ { 'Fix' : '");
        ble.print("0");
        ble.print("', 'Latitud' : '");  ble.printFloating(gdata.int_lat, gdata.dec_lat);
        ble.print("', 'Longitud' : '");  ble.printFloating(gdata.int_lon, gdata.dec_lon);
        ble.print("', 'Altura' : '");  ble.printFloating(gdata.int_altitude, gdata.dec_altitude);
        ble.println("' } ] }");
      }
  }
}

void V2Libs::sendLatitude() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.println(gdata.flat);
      } else {
        ble.println("-1");
      }
  }
}

void V2Libs::sendLongitude() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.println(gdata.flon);
      } else {
        ble.println("-1");
      }
  }
}

void V2Libs::sendAltitude() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.println(gdata.faltitude);
      } else {
        ble.println("-1");
      }
  }
}

long last_t = 0;  // Para loop()

void V2Libs::setup() {
    ble.setup(15, 14, 57600);  // Serial3 @ 57600
    ble.waitForConnection();
    
#ifdef OUTPUT_TO_PC
    pcComm.setup(0, 1, 57600); // Serial  @ 57600
    pcComm.waitForConnection();
#endif

    if (!tempSensor.setup()) {
#ifdef OUTPUT_TO_PC
        pcComm.println("Couldn't find MCP9808!");
#endif
        while (1); 
    }
  
    // Setup other stuff
    currentSensor.setup(A0);
    voltageSensor.setup(A1);

    gpsInput.setup(&Serial2, 9600);

/*
    // Setup SD
#ifdef OUTPUT_TO_PC
    pcComm.print("Initializing SD card...");
#endif
    if (!sdWriter.setup(38, &pcComm)) {
#ifdef OUTPUT_TO_PC
        pcComm.println("Error conexión SD.");
#endif
        while(1);
    }
#ifdef OUTPUT_TO_PC
    pcComm.println("initialization done.");
#endif
*/

#ifdef OUTPUT_TO_PC
    pcComm.println("Initializing RTC!");
#endif
    // Setup RTC
    if (!rtcClock.setup(&pcComm)) {
#ifdef OUTPUT_TO_PC
        pcComm.println("RTC is in bad shape!");
#endif
        while(1);
    }

#ifdef OUTPUT_TO_PC
    pcComm.println("RTC Done!");
#endif

    rowsCount = 0;
}

void V2Libs::loop() {
    gpsInput.loop();  // GPS
  
    static char cmdBuffer[BUFFER_MAX];
    static int cmdLen = 0;
    static char filename1[FILENAME_MAX+1];
    static char filename2[FILENAME_MAX+1];
  
    if (ble.available()) {
        char c = ble.read();
        cmdBuffer[cmdLen++] = c;
    
        if (cmdLen >= BUFFER_MAX || cmdBuffer[cmdLen-1] == '\n') {
      
        char *ptr;
        ptr = strtok(cmdBuffer," ");  //argumentos: comando, caracter delimitador
        while(ptr != NULL) {
            switch(unixCmd.parseCmd(cmdBuffer, cmdLen, filename1, filename2)) {
            case 0:
                unixCmd.ls();
                break;
            case 1:
                unixCmd.mv(filename1, filename2);
                break;
            case 2:
                unixCmd.rm(filename1);
                break;
            case 3:
                unixCmd.cat(filename1);
                break;
            case 4:
                unixCmd.touch(filename1);
                break;
            case 5:
                sendTemperature();
                break;
            case 6:
                sendCurrent();
                break;
            case 7:
                sendVoltage();
                break;
            case 8:
                sendGpsStruct();
                break;
            case 9:
                sendLatitude();
                break;
            case 10:
                sendLongitude();
                break;
          case 11:
                sendAltitude();
                break;
          default:
            ble.println("Command not found!");
#ifdef OUTPUT_TO_PC
            pcComm.println("Command not found!");
#endif
            break;
        }
        ptr = strtok(NULL, " ");
      }
      cmdLen = 0;
    }
    
  }

  // ########################## Sensors Handling ####################
  if (millis() - last_t >= 1000) {
  
    // RTC Time
    DateTime rtc_now = rtcClock.now();

    // Read and print out the temperature, then convert to *F
    float c = 0.0;//tempSensor.readCelsius();
    float f = c * 9.0 / 5.0 + 32;
#ifdef OUTPUT_TO_PC
    pcComm.print("Temp: "); pcComm.print(c); pcComm.print("*C\t"); 
    pcComm.print(f); pcComm.println("*F");
#endif
    
    pcComm.println("After temp read!");
    
    // Leer sensores de corriente y voltaje
    float vcurrent = toVolts(currentSensor.read());
    float current = toAmps(vcurrent);
#ifdef OUTPUT_TO_PC
    pcComm.print("Corriente: "); pcComm.print(current); pcComm.println("[A]");
#endif
    
    float voltage = toVolts(voltageSensor.read());
    float realv = (voltage * R1_R2) / R2;
#ifdef OUTPUT_TO_PC
    pcComm.print("Voltaje: "); pcComm.print(realv); pcComm.println("[V]");
#endif
    
    GpsStruct gdata;
    gdata.fix = 0;
    
    // ########################## GPS Handling ####################  
    if (gpsInput.available()) {
      gdata = gpsInput.getData();

#ifdef OUTPUT_TO_PC
      pcComm.print("Time: ");
      pcComm.printlnSimpleTime(gdata.hour, gdata.minute, gdata.second);
      
      pcComm.print("Date: ");
      pcComm.printlnSimpleDate(gdata.day, gdata.month, gdata.year);
      
      pcComm.print("Fix: "); pcComm.print((int)gdata.fix);
      pcComm.print(" quality: "); pcComm.println((int)gdata.quality);
#endif

      if (gdata.fix) {
#ifdef OUTPUT_TO_PC
        pcComm.print("Location: ");
        pcComm.print(gdata.flat);
        pcComm.print(", "); 
        pcComm.print(gdata.flon);
        
        pcComm.print("Speed (knots): "); pcComm.println(gdata.fspeed);
        pcComm.print("Angle: ");         pcComm.println(gdata.fangle);
        pcComm.print("Altitude: ");      pcComm.println(gdata.faltitude);
        pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
#endif
      }
    }
     
    // Inicializar archivo Tarjeta Sd
    if (rowsCount < 0) {  
#ifdef OUTPUT_TO_PC
        pcComm.println("Omitiendo escritura de archivo, dado que hay error previo...");
#endif    
    } else {
        // For new empty files, generate
        if (rowsCount == 0) {
            if(!sdWriter.open()) {
                rowsCount = -1;
            }
        }
        
        // Write data to file if no error is present
        if (rowsCount >= 0) {
            sdWriter.writeDatetime(rtc_now.year(), rtc_now.month(), rtc_now.day(), rtc_now.hour(), rtc_now.minute(), rtc_now.second());
            sdWriter.writeChar(';');

            sdWriter.writeFloat(c);
            sdWriter.writeChar(';');
            
            sdWriter.writeFloat(current);
            sdWriter.writeChar(';');
            
            sdWriter.writeFloat(realv);
            sdWriter.writeChar(';');

            // GPS Data

            sdWriter.writeChar(gdata.fix ? '1' : '0');
            sdWriter.writeChar(';');
            
            sdWriter.writeDatetime(gdata.year, gdata.month, gdata.day, gdata.hour, gdata.minute, gdata.day);
            sdWriter.writeChar(';');

            sdWriter.writeFloat(gdata.flat);
            sdWriter.writeChar(';');
            
            sdWriter.writeFloat(gdata.flon);
            sdWriter.writeChar(';');

            sdWriter.writeFloat(gdata.fspeed);
            sdWriter.writeChar(';');
            
            sdWriter.writeFloat(gdata.fangle);
            sdWriter.writeChar(';');
            
            sdWriter.writeFloat(gdata.faltitude);
            sdWriter.writeChar(';');
            
            sdWriter.writeInt32(gdata.satellites);
            sdWriter.writeChar(';');
            
            sdWriter.writeNewline();
            
            rowsCount++;
        }
        
        // Signal to renew the file in the next iteration.
        if (rowsCount == MAX_ROWS) {
            rowsCount = 0;
        }
    }
    
    last_t = millis();
  }
}
