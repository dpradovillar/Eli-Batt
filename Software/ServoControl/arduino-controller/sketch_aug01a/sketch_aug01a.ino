/** The MIT License (MIT)

Copyright (c) 2014, Rene Tapia <rtapiapincheira@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*
This program starts a serial communication with a host pc and a ttl serial device,
and serves as bridge for passing AT commands to the device. It was programmed
with a bluetooth module JY-MCU in mind, but it can be changed easily for others
as well.

The version 1.06 of the JY-MCU module requires a 3.3v RX/TX signal, which can be
addressed using a voltage divider or a logic level shifter. From the Arduino's side
no changes are required, just connect the 3.3v RX/TX from the bluetooth to any two
digital pins and change the constansts accordingly.

Beware for this code to work the bluetooth must be unpaired, because the AT commands
only work this way.

Author: Rene Tapia <rtapiapincheira@gmail.com>
*/
#include <SoftwareSerial.h>

#define PC_SERIAL_BAUD 9600
#define BT_SERIAL_BAUD 38400

#define RX_INPUT_PIN  9
#define TX_OUTPUT_PIN 8

SoftwareSerial btSerial(RX_INPUT_PIN, TX_OUTPUT_PIN);

void setup() { 
  Serial.begin(PC_SERIAL_BAUD); 
  while(!Serial);
  Serial.println("Connected to PC.");

  delay(1000);

  btSerial.begin(BT_SERIAL_BAUD);
} 

char c;
void loop() { 
  bool written = false;
  while(Serial.available()) {
    c = Serial.read();
    Serial.write(c);
    btSerial.write(c);
    written = true;
  }
  while (btSerial.available()) {
    Serial.write(btSerial.read());
  }
} 
