
#include <SPI.h>
#include <AdafruitSD.h>
#include <Wire.h>
#include <OneWire.h>
#include "Adafruit_MCP9808.h"
#include <RTClib.h>

// Create the MCP9808 temperature sensor object
//Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
RTC_Millis RTC;
File myFile;

const int pinSdChipSelect = 38;

OneWire  ds(51); // 18 or 19, Serial1 (BANK), 51 MOSI

void setup() {
  Serial.begin(9600);
  Serial.println("MCP9808 demo");

  // -------------------------------

  // following line sets the RTC to the date & time this sketch was compiled
  //DateTime sketch(__DATE__, __TIME__);
  //RTC.begin(DateTime (sketch.unixtime() + 20));

  // -------------------------------


  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(pinSdChipSelect, OUTPUT);

  //SPI.begin(); // wake up the SPI bus.
  
  //SPI.end();
  //SPI.setBitOrder(MSBFIRST);

  

  Serial.println("Starting wire library");
  Wire.begin();

 

  Serial.println("Wire library started!");



  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
/*  Serial.println("Starting temp sensor");
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
  Serial.println("Temp sensor started OK!");*/
  
  // -------------------------------------------------------------
  
}

void loop() {

  DateTime now = RTC.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print("  ");

  Serial.print(" seconds since 1970: ");
  Serial.println(now.unixtime());

  Serial.println();

  // -------------------------------
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

// ----------------------------------

  Serial.println("Waiting some time");

  delay(2500);
}
