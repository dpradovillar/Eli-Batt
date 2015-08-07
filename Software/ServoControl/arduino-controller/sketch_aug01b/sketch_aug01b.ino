#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX | TX

void setup()
{
  mySerial.begin(115200);
  
delay(1000);
mySerial.print("AT");
delay(1000);
mySerial.print("AT+VERSION");
delay(1000);
//mySerial.print("AT+PIN1234"); // Set pin to 1234
//delay(1000);
//mySerial.print("AT+NAMEJY-MCU-HC06"); // Set the name to JY-MCU-HC06
//delay(1000);
//mySerial.print("AT+BAUD8"); // Set baudrate to 115200
//mySerial.print(“AT+BAUD4″); // Set baudrate to 9600
//delay(1000);
  
}

void loop()
{
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (mySerial.available())
    Serial.write(mySerial.read());
    
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available()){
    char c = Serial.read();
    Serial.write(c);
    mySerial.write(c);
  }
}
