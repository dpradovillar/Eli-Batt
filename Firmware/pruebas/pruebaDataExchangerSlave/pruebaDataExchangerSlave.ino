#include <EEPROM.h>
#include <ArduinoSoftwareSerial.h>
#include <DataStream.h>
#include <DataExchanger.h>
#include <EepromWriter.h>
#include <Endpoint.h>
#include <SimpleCrc.h>
#include <Utils.h>

EepromWriter eepromWriter;
byte myId[4];

// previous in devices chain
SerialEndpoint commA;
DataStreamReader dsrA;
DataStreamWriter dswA;
  
// next in devices chain
SerialEndpoint commB;
DataStreamReader dsrB;
DataStreamWriter dswB;

// engine for crosstalk
DataExchanger dex;

int led = 13;

/**
 * Blinks a led 3 times whenever a message addressed to this board is received and processed.
 */
class LedBlinkHandler : public Handler {
public:
  bool handleMessage(Message *message) {
    for(int i=0; i<3; i++) {
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
      delay(100);
    }
    return false; // should not respond!
  }
};

LedBlinkHandler blinkHandler;

void setup() {
  pinMode(13, OUTPUT);
  
  // first thing, read id from eeprom
  uint32_t numId = eepromWriter.readId();
  Utils::toByte(numId, myId);
  
  // setup data exchanger only with references
  dex.setup(myId, &blinkHandler);
  dex.setupHardware(&dsrA, &dswA);
  dex.setupSoftware(&dsrB, &dswB);

  commA.setup(0, 1, 9600);
  commB.setup(6, 7, 9600);

  dsrA.setup(&commA); dswA.setup(&commA);
  dsrB.setup(&commB); dswB.setup(&commB);
  
  // wait for connection
  commA.waitForConnection();
  commB.waitForConnection();
}

void loop() {
  dex.loop();
}
