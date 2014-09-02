#include <EEPROM.h>
#include <ArduinoSoftwareSerial.h>

#include <DataStream.h>
#include <DataExchanger.h>
#include <EepromId.h>
#include <Endpoint.h>
#include <SimpleCrc.h>
#include <Utils.h>

EepromId eepromId;
byte myId[4];

// connection for debugging
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

SerialOutputHandler outputHandler;

void setup() {
  // first thing, read id from eeprom
  uint32_t numId = eepromId.readId();
  Utils::toByte(numId, myId);
  
  // setup data exchanger only with references
  dex.setup(myId, &outputHandler);
  dex.setupHardware(&dsrA, &dswA);
  dex.setupSoftware(&dsrB, &dswB);
  
  // start communications
  
  pcComm.setup(0, 1, 9600);

  commA.setup(6, 7, 9600);
  dsrA.setup(&commA);
  dswA.setup(&commA);
  
  commB.setup(8, 9, 9600);
  dsrB.setup(&commB);
  dswB.setup(&commB);
  
  // wait for connection
  pcComm.waitForConnection();
  commA.waitForConnection();
  commB.waitForConnection();
  
  // spit back some init parameters
  pcComm.print("Id read from eeprom:");
  pcComm.println((char*)myId, 4);
  pcComm.flush();
}

// buffer for setting the target address
byte targetId[4];

// buffer for setting the data
byte data[8];

void loop() {
  if (pcComm.available()) {
    char c = pcComm.read();
    // command to set the target id, needs 4 more bytes
    if (c == 't') {
      while(pcComm.available()<4);
      for(size_t i = 0; i < 4; i++) {
        targetId[i] = Serial.read();
      }
    }
    // command to set the data of the message, needs 8 more bytes
    else if (c == 'd') {
      while(pcComm.available()<8);
      for(size_t i = 0; i < 8; i++) {
        data[i] = Serial.read();
      }
    }
    // command to transmit the message
    else if (c == 's') {
      Message message;
      message.m_type = TYPE_DATA;
      message.m_status = 0; // currently unused.
      Utils::copyArray(myId, message.m_fromId, 4);
      Utils::copyArray(targetId, message.m_targetId, 4);
      Utils::copyArray(data, message.m_data, 8);
      dswB.writeObject(&message);
      dswA.writeObject(&message);
    }
  }
  dex.loop();
}
