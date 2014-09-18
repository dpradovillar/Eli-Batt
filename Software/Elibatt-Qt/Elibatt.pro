greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport webkitwidgets
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = elibatt
TEMPLATE = app

HEADERS += \
    serialthread.h \
    uidialog.h \
    connectionstatus.h \
    serialmonitor.h \
    uicontroller.h \
    eepromcontroller.h \
    utils.h \
    iddialog.h \
    gpscontroller.h \
    gpsdialog.h \
    messageserializer.h

SOURCES += \
    main.cpp \
    serialthread.cpp \
    uidialog.cpp \
    connectionstatus.cpp \
    serialmonitor.cpp \
    uicontroller.cpp \
    eepromcontroller.cpp \
    utils.cpp \
    iddialog.cpp \
    gpscontroller.cpp \
    gpsdialog.cpp \
    messageserializer.cpp

FORMS += \
    uidialog.ui \
    iddialog.ui \
    gpsdialog.ui

RC_FILE += res/icons.rc
