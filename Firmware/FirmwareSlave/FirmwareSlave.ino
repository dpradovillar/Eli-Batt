#include <ArduinoSoftwareSerial.h>

#include <Utils.h>
#include <SampleCollector.h>
#include <Endpoint.h>
#include <FirmwareSlave.h>
#include <DataStream.h>

FirmwareSlave firmware;
SerialEndpoint se;
SampleClock sClock;
//SerialEndpoint left;
//SerialEndpoint right;
DataStreamWriter dsw;

class Listener : public SampleCallback {
  void eventDetected(uint32_t current_usecs) {
    //se.println("var:", 7);
    dsw.writeString("Hola mundo!\n");
  }
};
Listener listener;

void setup() {
//    left.setup(8, 9);
//    right.setup(0, 1);

    firmware.setup();
    se.setup(0, 1, 9600);
    se.waitForConnection();
    
    sClock.setup(1, &listener);
    dsw.setup(&se);
}
void loop() {
    firmware.loop();
    sClock.loop();
}

