#ifndef __ELIBATT_FRAMEWORK_H__
#define __ELIBATT_FRAMEWORK_H__

/********************* Flags to enable/disable the compilation of certain features ****************/
// Defines whether the Elibatt Framework component objects will be included at compile time or not.

#define ENABLED_SAMPLE_COLLECTOR 1
#define ENABLED_EEPROM_ID        1
#define ENABLED_SIMPLE_CRC       1

#if ENABLED_EEPROM_ID
#include <EEPROM.h>
#endif

/*********************** Useful and convenient type definitions and macros  ***********************/

// This should'n be bigger than 8 or 12 bytes as the EEPROM is very small!
#define ID_LENGTH         8

#define ID_UNKNOWN        "00000000"
#define ID_EEPROM_OFFSET  1

// Every module should include this file

#endif // __ELIBATT_FRAMEWORK_H__
