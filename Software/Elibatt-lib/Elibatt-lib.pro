QT      += core serialport
QT      -= gui

TARGET   = elibatt-lib-test

CONFIG  += console
CONFIG  -= app_bundle

TARGET   = elibat-lib-test
TEMPLATE = app

HEADERS += seriallib.h   threadedserial.h   messageserializer.h
SOURCES += seriallib.cpp threadedserial.cpp messageserializer.cpp main.cpp
