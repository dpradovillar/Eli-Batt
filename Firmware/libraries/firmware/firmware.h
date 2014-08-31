#ifndef __FIRMWARE_H_
#define __FIRMWARE_H_

#ifdef FIRMWARE_MASTER
class MasterFirmware {
public:
    MasterFirmware();
}
#endif // FIRMWARE_MASTER

#ifdef FIRMWARE_SLAVE
class SlaveFirmware {
public:
     SlaveFirmware();
};
#endif // FIRMWARE_SLAVE

#endif // __FIRMWARE_H_
