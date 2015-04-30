#include <ArduinoSoftwareSerial.h>
#include <Endpoint.h>
#include <Utils.h>

SerialEndpoint pcComm;
SerialEndpoint bleComm;

DualSerialEndpoint dualComm;

/*
 Note: Make sure to run this sketch along with the Chat of Red Bear Lab (found on the playstore).
*/

void setup() {
  pcComm.setup(0, 1, 57600);    // Serial  @ 57600baud
  bleComm.setup(15, 14, 57600); // Serial3 @ 57600baud
  dualComm.setup(&bleComm, &pcComm); // pcComm will be used as a 'watcher' only
  
  bleComm.waitForConnection();
  pcComm.waitForConnection();
}

void loop() {
  while(dualComm.available()){
    
    // Reads from BLE
    char c = dualComm.read();
    
    // Writes back to BLE (and PC)
    dualComm.write(c);
  }
}
