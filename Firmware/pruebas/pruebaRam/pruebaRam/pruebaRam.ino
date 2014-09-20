#include <AnalogInput.h>

#include <MemoryFree.h>
//#include <SD.h>

#define OUTPUT_FM {Serial.print(freeMemory()); Serial.println("FM");}

//File root;

AnalogInput ai;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
ai.setup(13);

Serial.print(sizeof(int8_t)*8); Serial.println("bits");
Serial.print(sizeof(int16_t)*8); Serial.println("bits");
Serial.print(sizeof(int32_t)*8); Serial.println("bits");
Serial.print(sizeof(int)*8); Serial.println("bits");
Serial.print(sizeof(short)*8); Serial.println("bits");
Serial.print(sizeof(long)*8); Serial.println("bits");
Serial.print(sizeof(long long)*8); Serial.println("bits");

  OUTPUT_FM;

  //Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  //pinMode(10, OUTPUT);

  /*if (SD.begin(38)) {
    Serial.println("initialization done.");
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("done!");
  } else {
     Serial.println("initialization failed!");
  }*/
  
  OUTPUT_FM;
}

void loop()
{
  // nothing happens after setup finishes.
}
/*
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
*/

