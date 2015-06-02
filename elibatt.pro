QT -= core
QT -= gui

TARGET  = main
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ./Firmware/libraries/arduino_serial \
    ./Firmware/libraries/MCP9808 \
    ./Firmware/libraries/AdafruitSD \
    ./Firmware/libraries/Config \
    ./Firmware/libraries/EepromWriter \
    ./Firmware/libraries/SensorInput \
    ./Firmware/libraries/DataExchanger \
    ./Firmware/libraries/Elibatt_RBL_BLEMini \
    ./Firmware/libraries/pin-mapping.pdf \
    ./Firmware/libraries/DS3231 \
    ./Firmware/libraries/V2Libs \
    ./Firmware/libraries/Adafruit_GPS \
    ./Firmware/libraries/ArduinoSoftwareSerial \
    ./Firmware/libraries/pin-mapping.xls \
    ./Firmware/libraries/libraries \
    ./Firmware/libraries/SdData \
    ./Firmware/libraries/Cmd \
    ./Firmware/libraries/RTClib \
    ./Firmware/libraries/readme.txt \
    ./Firmware/libraries/AltSoftSerial \
    ./Firmware/libraries/Utils \
    ./Firmware/libraries/OneWire \
    ./Firmware/libraries/MemoryFree \
    ./Firmware/libraries/.DS_Store \
    ./Firmware/libraries/Firmware
HEADERS += ./Firmware/libraries/AdafruitSD/utility/SdFatmainpage.h \
    ./Firmware/libraries/DS3231/DS3231.h \
    ./Firmware/libraries/AltSoftSerial/config/known_boards.h \
    ./Firmware/libraries/OneWire/OneWire.h \
    ./Firmware/libraries/EepromWriter/EepromWriter.h \
    ./Firmware/libraries/SensorInput/SampleCollector.h \
    ./Firmware/libraries/Config/elibatt_config.h \
    ./Firmware/libraries/RTClib/RTClib.h \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/ble_mini.h \
    ./Firmware/libraries/Firmware/FirmwareSlave.h \
    ./Firmware/libraries/AdafruitSD/utility/FatStructs.h \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/examples/BLEControllerSketch/Boards.h \
    ./Firmware/libraries/RTClib/RTC_DS3231.h \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/examples/BLEFirmataSketch/BLEFirmata.h \
    ./Firmware/libraries/SdData/SdData.h \
    ./Firmware/libraries/Utils/SimpleCrc.h \
    ./Firmware/libraries/AdafruitSD/utility/SdFat.h \
    ./Firmware/libraries/ArduinoSoftwareSerial/ArduinoSoftwareSerial.h \
    ./Firmware/libraries/AltSoftSerial/config/AltSoftSerial_Boards.h \
    ./Firmware/libraries/SensorInput/AnalogInput.h \
    ./Firmware/libraries/SensorInput/I2cInput.h \
    ./Firmware/libraries/SensorInput/RtcInput.h \
    ./Firmware/libraries/AdafruitSD/utility/SdFatUtil.h \
    ./Firmware/libraries/V2Libs/V2Libs.h \
    ./Firmware/libraries/arduino_serial/MessageBuilder.h \
    ./Firmware/libraries/Cmd/Cmd.h \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/examples/BLEFirmataSketch/Boards.h \
    ./Firmware/libraries/Firmware/Firmware.h \
    ./Firmware/libraries/AltSoftSerial/AltSoftSerial.h \
    ./Firmware/libraries/arduino_serial/EngineProtocol.h \
    ./Firmware/libraries/Utils/Utils.h \
    ./Firmware/libraries/SensorInput/OneWireInput.h \
    ./Firmware/libraries/AdafruitSD/AdafruitSD.h \
    ./Firmware/libraries/AltSoftSerial/config/known_timers.h \
    ./Firmware/libraries/DataExchanger/Debugger.h \
    ./Firmware/libraries/Adafruit_GPS/Adafruit_GPS.h \
    ./Firmware/libraries/AdafruitSD/utility/SdInfo.h \
    ./Firmware/libraries/MemoryFree/MemoryFree.h \
    ./Firmware/libraries/AltSoftSerial/config/AltSoftSerial_Timers.h \
    ./Firmware/libraries/RTClib/RTC_DS3234.h \
    ./Firmware/libraries/MCP9808/Adafruit_MCP9808.h \
    ./Firmware/libraries/AdafruitSD/utility/Sd2PinMap.h \
    ./Firmware/libraries/AdafruitSD/utility/Sd2Card.h \
    ./Firmware/libraries/SensorInput/GpsInput.h \
    ./Firmware/libraries/SdData/BankData.h \
    ./Firmware/libraries/DataExchanger/Endpoint.h \
    ./Firmware/libraries/Firmware/FirmwareMaster.h \
    ./Firmware/libraries/arduino_serial/Message.h \
    ./Firmware/libraries/DataExchanger/DataExchanger.h \
    ./Firmware/libraries/RTClib/RTC_DS1307.h \
    ./Firmware/libraries/DataExchanger/DataStream.h
SOURCES += ./Firmware/libraries/RTClib/RTC_DS3234.cpp \
    ./Firmware/libraries/SensorInput/AnalogInput.cpp \
    ./Firmware/libraries/arduino_serial/MessageBuilder.cpp \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/ble_mini.cpp \
    ./Firmware/libraries/EepromWriter/EepromWriter.cpp \
    ./Firmware/libraries/OneWire/OneWire.cpp \
    ./Firmware/libraries/SensorInput/I2cInput.cpp \
    ./Firmware/libraries/DataExchanger/Endpoint.cpp \
    ./Firmware/libraries/RTClib/RTClib.cpp \
    ./Firmware/libraries/RTClib/RTC_DS1307.cpp \
    ./Firmware/libraries/arduino_serial/EngineProtocol.cpp \
    ./Firmware/libraries/MemoryFree/MemoryFree.cpp \
    ./Firmware/libraries/AdafruitSD/utility/Sd2Card.cpp \
    ./Firmware/libraries/AltSoftSerial/AltSoftSerial.cpp \
    ./Firmware/libraries/RTClib/RTC_DS3231.cpp \
    ./Firmware/libraries/Firmware/Firmware.cpp \
    ./Firmware/libraries/AdafruitSD/File.cpp \
    ./Firmware/libraries/DataExchanger/DataStream.cpp \
    ./Firmware/libraries/Elibatt_RBL_BLEMini/examples/BLEFirmataSketch/BLEFirmata.cpp \
    ./Firmware/libraries/ArduinoSoftwareSerial/ArduinoSoftwareSerial.cpp \
    ./Firmware/libraries/Utils/SimpleCrc.cpp \
    ./Firmware/libraries/Utils/Utils.cpp \
    ./Firmware/libraries/Firmware/FirmwareSlave.cpp \
    ./Firmware/libraries/DS3231/DS3231.cpp \
    ./Firmware/libraries/DataExchanger/Debugger.cpp \
    ./Firmware/libraries/Adafruit_GPS/Adafruit_GPS.cpp \
    ./Firmware/libraries/SensorInput/RtcInput.cpp \
    ./Firmware/libraries/AdafruitSD/AdafruitSD.cpp \
    ./Firmware/libraries/SensorInput/SampleCollector.cpp \
    ./Firmware/libraries/MCP9808/Adafruit_MCP9808.cpp \
    ./Firmware/libraries/SensorInput/GpsInput.cpp \
    ./Firmware/libraries/V2Libs/V2Libs.cpp \
    ./Firmware/libraries/Cmd/Cmd.cpp \
    ./Firmware/libraries/SdData/SdData.cpp \
    ./Firmware/libraries/AdafruitSD/utility/SdVolume.cpp \
    ./Firmware/libraries/SdData/BankData.cpp \
    ./Firmware/libraries/Firmware/FirmwareMaster.cpp \
    ./Firmware/libraries/DataExchanger/DataExchanger.cpp \
    ./Firmware/libraries/AdafruitSD/utility/SdFile.cpp \
    ./Firmware/libraries/SensorInput/OneWireInput.cpp


INCLUDEPATH += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/EEPROM \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SPI \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SoftwareSerial
HEADERS += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SPI/SPI.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire/Wire.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/EEPROM/EEPROM.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire/utility/twi.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SoftwareSerial/SoftwareSerial.h
SOURCES += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SoftwareSerial/SoftwareSerial.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SPI/SPI.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire/utility/twi.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire/Wire.cpp


INCLUDEPATH += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino
HEADERS += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/USBDesc.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/USBCore.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Print.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Arduino.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/WString.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Stream.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial_private.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/USBAPI.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/new.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Printable.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Client.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Server.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/WCharacter.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Udp.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring_private.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/IPAddress.h \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/binary.h
SOURCES += /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring_analog.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/WString.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/hooks.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial1.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial3.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/IPAddress.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/main.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring_shift.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/new.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HID.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/WMath.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring_digital.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/USBCore.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/CDC.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Stream.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/abi.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/WInterrupts.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Tone.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Print.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/wiring_pulse.c \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial0.cpp \
    /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/HardwareSerial2.cpp

