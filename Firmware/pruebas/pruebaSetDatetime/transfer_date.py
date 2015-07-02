#!/usr/bin/python

import datetime
import serial
import struct
import time

def getBinaryDate():
    now = datetime.datetime.now()
    (year,month,day, hour,minute,second) = (now.year-2000,now.month,now.day, now.hour,now.minute,now.second)
    t = (year,month,day,hour,minute,second)
    print t
    return ''.join([struct.pack('!B',v) for v in t])

ser = serial.Serial('/dev/tty.usbserial-AH02F1CR', 57600)

time.sleep(2) # pause for 1 sec

ser.write(getBinaryDate());
while True:
    s = ser.read()
    print s,
ser.close()
