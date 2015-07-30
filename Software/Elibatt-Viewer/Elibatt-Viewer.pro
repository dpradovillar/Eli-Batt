QT       += widgets core printsupport

TARGET = elibatt-viewer
TEMPLATE = app

RC_FILE += res/icons.rc

FORMS += mainwindow.ui \
    aboutdialog.ui

HEADERS += \
    mainwindow.h \
    reportgenerator.h \
    filescanner.h

SOURCES += \
    mainwindow.cpp \
    reportgenerator.cpp \
    filescanner.cpp \
    main.cpp

OTHER_FILES += \
    res/icons.rc

RESOURCES += resources.qrc
