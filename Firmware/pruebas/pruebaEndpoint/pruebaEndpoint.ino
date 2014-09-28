#include <ArduinoSoftwareSerial.h>
#include <Utils.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <SimpleCrc.h>

SerialEndpoint pcComm;
SerialEndpoint softComm;

void setup() {
  // Initialize connection via hardware (to pc, Serial)
  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();
  
  // Initialize connection via software (to arduino, SoftwareSerial)
  softComm.setup(8, 9, 9600);
  softComm.waitForConnection();
}

void loop() {
  while(pcComm.available()) {
    byte b = pcComm.read();
    if (b == 'i') {
      softComm.print(9);
      softComm.println(9);
      softComm.println();
      softComm.print('1');
      softComm.println('1');
      softComm.print("hola!", 3);
      softComm.println("hola!", 3);
      char buffer[4] = {'a', 'b', 'c', 'd'};
      softComm.print(buffer, 2);
      softComm.println(buffer, 2);
      softComm.flush();
    }
    softComm.write(b);
  }
  
  while(softComm.available()){
    byte b = softComm.read();
    //pcComm.write(b);
    Serial.write(b);
  }
}
