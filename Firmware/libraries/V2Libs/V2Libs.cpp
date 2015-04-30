#include <V2Libs.h>

V2Libs::V2Libs() {
    //countRows = 0;
}

V2Libs::~V2Libs() {
}

void V2Libs::sendTemperature() {
  float c = tempSensor.readCelsius();   // Grados Celcius
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
        ble.print("', 'Latitud' : '");  ble.print(gdata.int_lat);ble.print(".");ble.print(gdata.dec_lat);
        ble.print("', 'Longitud' : '");  ble.print(gdata.int_lon);ble.print(".");ble.print(gdata.dec_lon);
        ble.print("', 'Altura' : '");  ble.print(gdata.int_altitude);ble.print(".");ble.print(gdata.dec_altitude);
        ble.println("' } ] }");
      } else {
        ble.print("{'GPS': [ { 'Fix' : '");
        ble.print("0");
        ble.print("', 'Latitud' : '");  ble.print(gdata.int_lat);ble.print(".");ble.print(gdata.dec_lat);
        ble.print("', 'Longitud' : '");  ble.print(gdata.int_lon);ble.print(".");ble.print(gdata.dec_lon);
        ble.print("', 'Altura' : '");  ble.print(gdata.int_altitude);ble.print(".");ble.print(gdata.dec_altitude);
        ble.println("' } ] }");
      }
  }
}

void V2Libs::sendLatitude() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.print(gdata.int_lat);
        ble.print(".");
        ble.println(gdata.dec_lat);
      } else {
        ble.println("-1");
      }
  }
}

void V2Libs::sendLongitude() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.print(gdata.int_lon);
        ble.print(".");
        ble.println(gdata.dec_lon);
      } else {
        ble.println("-1");
      }
  }
}

void V2Libs::sendHeight() {
  if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      if (gdata.fix) {
        ble.print(gdata.int_altitude);
        ble.print(".");
        ble.println(gdata.dec_altitude);
      } else {
        ble.println("-1");
      }
  }
}

long last_t = 0;  // Para loop()

void V2Libs::setup() {
    ble.setup(15, 14, 57600);  // Serial3 @ 57600
    pcComm.setup(0, 1, 9600); // Serial  @ 9600

    ble.waitForConnection();
    pcComm.waitForConnection();

    if (!tempSensor.setup()) {
        pcComm.println("Couldn't find MCP9808!");
        while (1); 
    }
  
    // Setup other stuff
    currentSensor.setup(A0);
    voltageSensor.setup(A1);

    gpsInput.setup(&Serial2, 9600);
  
    // Setup SD
    /*
    pcComm.print("Initializing SD card...");
    if (!SD.begin(38)) {
        pcComm.println("Error conexión SD.");
    }
    pcComm.println("initialization done.");
    */
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
                sendHeight();
                break;
          default:
            ble.println("Command not found!");
            ble.println("############################");
            break;
        }
        ptr = strtok(NULL, " ");
      }
      cmdLen = 0;
    }
    
  }

  // ########################## Sensors Handling ####################
  if (millis() - last_t >= 2000) {    
    // Read and print out the temperature, then convert to *F
    float c = tempSensor.readCelsius();
    float f = c * 9.0 / 5.0 + 32;
    //pcComm.print("Temp: "); pcComm.print(c); pcComm.print("*C\t"); 
    //pcComm.print(f); pcComm.println("*F");
    
    // Leer sensores de corriente y voltaje
    float vcurrent = toVolts(currentSensor.read());
    float current = toAmps(vcurrent);
    //pcComm.print("Corriente: "); pcComm.print(current); pcComm.println("[A]");
    
    float voltage = toVolts(voltageSensor.read());
    float realv = (voltage * R1_R2) / R2;
    //pcComm.print("Voltaje: "); pcComm.print(realv); pcComm.println("[V]");
    
    // Valores GPS
    float latitud = 0;
    float longitud = 0;
    float altura = 0;
    
    // ########################## GPS Handling ####################  
    if (gpsInput.available()) {
      GpsStruct gdata = gpsInput.getData();
      //pcComm.print("Time: ");
      //pcComm.printlnSimpleTime(gdata.hour, gdata.minute, gdata.second);
      
      //pcComm.print("Date: ");
      //pcComm.printlnSimpleDate(gdata.day, gdata.month, gdata.year);
      
      //pcComm.print("Fix: "); pcComm.print((int)gdata.fix);
      //pcComm.print(" quality: "); pcComm.println((int)gdata.quality);
      
      if (gdata.fix) {                        
        pcComm.print("Location: ");
        pcComm.printFloating(gdata.int_lat, gdata.dec_lat);
        pcComm.print(", "); 
        pcComm.printFloating(gdata.int_lon, gdata.dec_lon);
        
        pcComm.print("Speed (knots): "); pcComm.printlnFloating(gdata.int_speed, gdata.dec_speed);
        pcComm.print("Angle: ");         pcComm.printlnFloating(gdata.int_angle, gdata.dec_angle);
        pcComm.print("Altitude: ");      pcComm.printlnFloating(gdata.int_altitude, gdata.dec_altitude);
        pcComm.print("Satellites: ");    pcComm.println((int)gdata.satellites);
      }
    }
     
    // Inicializar archivo Tarjeta Sd
    /*
    if (countRows == 0) {
        pcComm.print("Initializing test...txt");
        char filename[] = "test...txt";
        filename[4] = ('0' + (countRows / 10) % 10);
        filename[5] = ('0' + countRows % 10);
        myFile.close();
        myFile = SD.open(filename, FILE_WRITE);
        if (!myFile) {
            // if the file didn't open, print an error:
            pcComm.println("Error opening test...txt");
        } else {
            pcComm.println("Abriendo test...txt:");
        }
    }*/
    
    // ###### Grabar en la SD ########
    // -> VOLTAGE, CURRENT, TEMPERATURE, LATITUD, LONGITUD, ALTURA  ######
      
    // Voltage 3.3v --> to 12v
    //myFile.print(voltage); myFile.print(";");
    //myFile.print(realv);     myFile.print(";");
    
    // Voltage 3.3v --> to amps
    //myFile.print(vcurrent);myFile.print(";");
    //myFile.print(current);   myFile.print(";");
    
    // Temperatura
    //myFile.print(c);         myFile.print(";");
    
    // Latitud
    //myFile.print(latitud);   myFile.print(";");

    // Longitud
    //myFile.print(longitud);  myFile.print(";");
     
    // Altura
    //myFile.print(altura);    myFile.print(";");
     
    //myFile.println();
  
    //countRows++;
    //if (countRows == MAX_ROWS) {
    //  countRows = 0;
    //}
    
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
    
    //pcComm.println("############################");
    
    last_t = millis();
  }
}