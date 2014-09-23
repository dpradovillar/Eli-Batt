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

SerialEndpoint pcComm;

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

//SerialOutputHandler outputHandler;

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

SerialOutputHandler outputHandler;
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

  pcComm.setup(0, 1, 9600);
  pcComm.waitForConnection();
  // spit back some init parameters
  pcComm.print("Id read from eeprom:");
  pcComm.println((char*)myId, 4);
  pcComm.flush();

  commA.setup(6, 7, 9600);
  commB.setup(8, 9, 9600); // Currently unused as the master is the first in the chain

  dsrA.setup(&commA); dswA.setup(&commA);
  dsrB.setup(&commB); dswB.setup(&commB);
  
  // wait for connection
  commA.waitForConnection();
  commB.waitForConnection();
}

void loop() {
  // buffer for setting the target address
  static byte targetId[4];
  // buffer for setting the data
  static byte data[8];

  if (pcComm.available()) {
    char c = pcComm.read();
    // command to set the target id, needs 4 more bytes
    if (c == 't') {
      Serial.print("Reading id, input 4 more bytes:");
      for(size_t i = 0; i < 4; i++) {
        while(!pcComm.available());
        targetId[i] = Serial.read();
        Serial.print((char)targetId[i]);
        Serial.flush();
      }
      Serial.println();
      Serial.println("id read correcly!");
    }
    // command to set the data of the message, needs 8 more bytes
    else if (c == 'd') {
      Serial.print("Reading data, input 8 more bytes:");
      for(size_t i = 0; i < 8; i++) {
        while(!pcComm.available());
        data[i] = Serial.read();
        Serial.print((char)data[i]);
        Serial.flush();
      }
      Serial.println();
      Serial.println("data read correcly!");
    }
    // command to transmit the message
    else if (c == 's') {
      Message message;
      message.m_type = TYPE_DATA;
      message.m_status = 0; // currently unused.
      Utils::copyArray(myId, message.m_fromId, 4);
      Utils::copyArray(targetId, message.m_targetId, 4);
      Utils::copyArray(data, message.m_data, 8);
      message.m_crc = message.calculateCrc();
      
      Serial.println("Message contents being sent:");
      outputHandler.handleMessage(&message);
      
      Serial.println("Sending message through both endpoints!");
      dswB.writeObject(&message);
      dswB.flush();
      dswA.writeObject(&message);
      dswA.flush();
    }
  }
  dex.loop();
}
