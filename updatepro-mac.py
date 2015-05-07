#!/usr/bin/python

template = '''QT -= core
QT -= gui

TARGET  = main
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
%s
%s
%s
'''

FILE_SYSTEM = '''
INCLUDEPATH += %s
HEADERS += %s
SOURCES += %s
'''

import os
import sys

def getPathFor(rootdir, filter, level):
    dir = []
    header = []
    source = []
    for root, subFolders, files in os.walk(rootdir):
        for file in files:
            path = os.path.join(root,file).replace('\\', '/');
            spath = (path if not ' ' in path else '"'+path+'"');
            if (filter is None) or (filter in path):
                if path.endswith('.h'):
                    header.append(spath);
                elif path.endswith('.cpp') or path.endswith('.c'):
                    source.append(spath);
                dir.append('/'.join(path.split('/')[0:level]));
    return (dir,header,source);

def makeReadable(li):
    return ' \\\n    '.join(list(set(li)));

(dir,header,source) = getPathFor('.', 'Firmware/libraries', 4);
FIRMWARE = FILE_SYSTEM %(
    makeReadable(dir),
    makeReadable(header),
    makeReadable(source)
)

(dir,header,source)=getPathFor('/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries', None, 10)
ARDUINO = FILE_SYSTEM %(
    makeReadable(dir),
    makeReadable(header),
    makeReadable(source)
)

(dir,header,source)=getPathFor('/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino', None, 10)
CORE = FILE_SYSTEM %(
    makeReadable(dir),
    makeReadable(header),
    makeReadable(source)
)

s = template % (FIRMWARE, ARDUINO, CORE)
#print CORE
#print s
open('elibatt.pro','w').write(s);
