#include <Wire.h>
#include <Adafruit_MCP9808.h>
#include <I2cInput.h>
#include <AnalogInput.h>

Mcp9808Sensor tempSensor;
AnalogInput currentSensor;
AnalogInput voltageSensor;

void setup() {
  // Setup serial communication
  Serial.begin(9600);
  while(!Serial);
  
  // Setup other stuff
  tempSensor.setup(); // uses A4 and A5
  currentSensor.setup(A0);
  voltageSensor.setup(A1);
}

void loop() {
Serial.print(tempSensor.readCelsius());Serial.print(";");
Serial.print((int)currentSensor.read());Serial.print(";");
Serial.print((int)voltageSensor.read());Serial.print(";");
  Serial.println();
  delay(1000);
}
