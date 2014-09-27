/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 */

#include <OneWire.h>

#include <SD.h>

int DS18S20_Pin = 10; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = A1;  // Analog input pin that the potentiometer is attached to

const int numReadings1 = 20;
const int numReadings2 = 10;

int readings1[numReadings1];      // the readings from the analog input
int index1 = 0;                  // the index of the current reading
int total1 = 0;                  // the running total
int average1 = 0;                // the average

int readings2[numReadings2];      // the readings from the analog input
int index2 = 0;                  // the index of the current reading
int total2 = 0;                  // the running total
int average2 = 0;                // the average

const int chipSelect = 8;

const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue1 = 0;        // value read from the pot
int sensorValue2 = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
    // initialize all the readings to 0: 
  for (int thisReading1 = 0; thisReading1 < numReadings1; thisReading1++)
    readings1[thisReading1] = 0;   
    
       // initialize all the readings to 0: 
  for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++)
    readings2[thisReading2] = 0;   
    
      Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
}

void loop() {

  
  // read the analog in value:
  sensorValue1 = analogRead(analogInPin1);      
  sensorValue2 = analogRead(analogInPin2);  

 // subtract the last reading:
  total1= total1 - readings1[index1];         
  // read from the sensor:  
  readings1[index1] = analogRead(analogInPin1); 
  // add the reading to the total:
  total1= total1 + readings1[index1];       
  // advance to the next position in the array:  
  index1 = index1 + 1;                    

  // if we're at the end of the array...
  if (index1 >= numReadings1)              
    // ...wrap around to the beginning: 
    index1 = 0;                           

  // calculate the average:
  average1 = total1 / numReadings1;         
  // send it to the computer as ASCII digits

  delay(1);        // delay in between reads for stability   
  
  
   // subtract the last reading:
  total2= total2 - readings2[index2];         
  // read from the sensor:  
  readings2[index2] = analogRead(analogInPin2); 
  // add the reading to the total:
  total2= total2 + readings2[index2];       
  // advance to the next position in the array:  
  index2 = index2 + 1;                    

  // if we're at the end of the array...
  if (index2 >= numReadings2)              
    // ...wrap around to the beginning: 
    index2 = 0;                           

  // calculate the average:
  average2 = total2 / numReadings2;         
  // send it to the computer as ASCII digits

  delay(1);        // delay in between reads for stability   
  
  

  float temperature = getTemp();
  
   // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:

  dataString += String(average1);
  dataString += ","; 
   dataString += String(average2);
  dataString += ","; 
   dataString += String(int(temperature));
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }  
   
   
  
 
 

  // print the results to the serial monitor:                     
  Serial.print(average1);   
  Serial.print(',');   
  Serial.print(average2); 
  Serial.print(',');  
  Serial.println(temperature);
 

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                     
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
