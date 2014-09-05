#include <Servo.h> 
#include <SoftwareSerial.h>

SoftwareSerial bt232(9, 8); // RX, TX

#define pinServo1 5
#define pinServo2 6
Servo servo1, servo2;
 
bool getPos1;
 
void setup() 
{ 
  //Serial.begin(9600); 
  //while(!Serial);

  servo1.attach(pinServo1);
  servo2.attach(pinServo2);

  bt232.begin(9600);
  
  getPos1 = true;
} 

void loop() 
{ 
  while (bt232.available()) {
    if (getPos1) {
      int p1 = (int)(bt232.read() & 0xFF);
      //Serial.print("Received pos1:"); Serial.println(p1);
      servo1.write(p1);
      getPos1 = false;
    } else {
      int p2 = (int)(bt232.read() & 0xFF);
      //Serial.print("Received pos2:"); Serial.println(p2);
      servo2.write(p2);
      getPos1 = true;
    }
  }
} 
