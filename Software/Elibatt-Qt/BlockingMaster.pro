greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = blockingmaster
TEMPLATE = app

HEADERS += \
    dialog.h \
    masterthread.h \
    uidialog.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    masterthread.cpp \
    uidialog.cpp

FORMS += \
    uidialog.ui