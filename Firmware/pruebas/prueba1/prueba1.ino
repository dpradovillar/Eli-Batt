#include <SampleCollector.h>

SerialCallback serialCallback;
LedBlinkCallback ledBlinkCallback;

SampleClock sampleClock;
SampleClock ledClock;

void setup() {
    Serial.begin(9600);
    sampleClock.setup(1, &serialCallback);

    ledBlinkCallback.setup(13);
    ledClock.setup(2, &ledBlinkCallback);
}

void loop() {
    sampleClock.loop();
    ledClock.loop();
}
