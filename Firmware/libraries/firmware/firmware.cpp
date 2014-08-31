#include "libraries/firmware/firmware.h"

#ifdef FIRMWARE_MASTER
MasterFirmware::MasterFirmware() {
}
#endif // FIRMWARE_MASTER


#ifdef FIRMWARE_SLAVE
SlaveFirmware::SlaveFirmware() {
}
#endif // FIRMWARE_SLAVE
