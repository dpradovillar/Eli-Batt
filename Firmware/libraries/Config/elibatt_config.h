#ifndef __ELIBATT_CONFIG_H_
#define __ELIBATT_CONFIG_H_

// Available boards for development:
#define BOARD_UNO  0x01
#define BOARD_MEGA 0x02

// Available types of firmware:
#define FIRMWARE_MASTER 0xA
#define FIRMWARE_SLAVE  0xB

// Select the board used for the firmware:
#ifndef TARGET_BOARD
//#  define TARGET_BOARD BOARD_UNO
#  define TARGET_BOARD BOARD_MEGA
#endif

// Select the type of firmware for building:
#ifndef TARGET_FIRMWARE
//#  define TARGET_FIRMWARE FIRMWARE_SLAVE
#  define TARGET_FIRMWARE FIRMWARE_MASTER
#endif

// Select the profile used for the firmware:
#ifndef TARGET_DEBUG
//#  define TARGET_DEBUG true
#  define TARGET_DEBUG false
#endif

#ifdef DEBUG_RX
# error "DEBUG_RX symbol already defined."
#endif
#ifdef DEBUG_TX
# error "DEBUG_TX symbol already defined."
#endif
#ifdef COMM1_RX
# error "COMM1_RX symbol already defined."
#endif
#ifdef COMM1_TX
# error "COMM1_TX symbol already defined."
#endif
#ifdef COMM2_RX
# error "COMM2_RX symbol already defined."
#endif
#ifdef COMM2_TX
# error "COMM2_TX symbol already defined."
#endif
#ifdef GPS_RX
# error "GPS_RX symbol already defined."
#endif
#ifdef GPS_TX
# error "GPS_TX symbol already defined."
#endif
#ifdef BLE_RX
# error "BLE_RX symbol already defined."
#endif
#ifdef BLE_TX
# error "BLE_TX symbol already defined."
#endif
#ifdef CURRENT_PIN
# error "CURRENT_PIN symbol already defined."
#endif
#ifdef VOLTAGE_PIN
# error "VOLTAGE_PIN symbol already defined."
#endif

// Pins common to all boards/profiles
#define CURRENT_PIN A0
#define VOLTAGE_PIN A1

// Number of rows of each file (duration in seconds)
//#define FILE_DURATION 21600 // 6hrs
#define FILE_DURATION 120 // 2 min

// Delay for master sample clock in microseconds
#define MASTER_CLOCK_DELAY 1000000 // 1 sec

#define COMM1_SPEED 9600  // Hardware-output
#define COMM2_SPEED 9600  // Software-output
#define DEBUG_SPEED 9600 // PC comm

#define DEBUG_PIN 13

// Specific combination handling
#if TARGET_BOARD==BOARD_UNO || TARGET_BOARD==BOARD_MEGA
#  if TARGET_BOARD==BOARD_UNO
#    define SD_CS_PIN        4
#    if TARGET_FIRMWARE==FIRMWARE_MASTER
#      if TARGET_DEBUG==true
//       UNO/MASTER/DEBUG
#        define PROFILE_LABEL "UNO/MASTER/DEBUG"
#        define DEBUG_RX     0
#        define DEBUG_TX     1
#        define COMM2_RX     6
#        define COMM2_TX     7
#        define GPS_RX       2
#        define GPS_TX       3
#        define BLE_RX       4
#        define BLE_TX       5
#      else
//       UNO/MASTER/RELEASE
#        define PROFILE_LABEL "UNO/MASTER/RELEASE"
#        define COMM2_RX     0
#        define COMM2_TX     1
#        define GPS_RX       2
#        define GPS_TX       3
#        define BLE_RX       4
#        define BLE_TX       5
#      endif
#    elif TARGET_FIRMWARE==FIRMWARE_SLAVE
#      if TARGET_DEBUG==true
//       UNO/SLAVE/DEBUG
#        define PROFILE_LABEL "UNO/SLAVE/DEBUG"
#        define DEBUG_RX     0
#        define DEBUG_TX     1
#        define COMM1_RX     8
#        define COMM1_TX     9
#        define COMM2_RX     6
#        define COMM2_TX     7
#      else
//       UNO/SLAVE/RELEASE
#        define PROFILE_LABEL "UNO/SLAVE/RELEASE"
#        define COMM1_RX     0
#        define COMM1_TX     1
#        define COMM2_RX     6
#        define COMM2_TX     7
#      endif
#    endif
#  elif TARGET_BOARD==BOARD_MEGA
#    define SD_CS_PIN        38
#    if TARGET_FIRMWARE==FIRMWARE_MASTER
#      if TARGET_DEBUG==true
//       MEGA/MASTER/DEBUG
#        define PROFILE_LABEL "MEGA/MASTER/DEBUG"
#        define DEBUG_RX     0
#        define DEBUG_TX     1
#        define COMM2_RX     19
#        define COMM2_TX     18
#        define GPS_RX       17
#        define GPS_TX       16
#        define BLE_RX       15
#        define BLE_TX       14
#      else
//       MEGA/MASTER/RELEASE
#        define PROFILE_LABEL "MEGA/MASTER/RELEASE"
#        define COMM2_RX     19
#        define COMM2_TX     18
#        define GPS_RX       17
#        define GPS_TX       16
#        define BLE_RX       15
#        define BLE_TX       14
#      endif
#    elif TARGET_FIRMWARE==FIRMWARE_SLAVE
#      if TARGET_DEBUG==true
//       MEGA/SLAVE/DEBUG
#        define PROFILE_LABEL "MEGA/SLAVE/DEBUG"
#        define DEBUG_RX     0
#        define DEBUG_TX     1
#        define COMM1_RX     17
#        define COMM1_TX     16
#        define COMM2_RX     19
#        define COMM2_TX     18
#      else
//       MEGA/SLAVE/RELEASE
#        define PROFILE_LABEL "MEGA/SLAVE/RELEASE"
#        define COMM1_RX     17
#        define COMM1_TX     16
#        define COMM2_RX     19
#        define COMM2_TX     18
#      endif
#    endif
#  endif

// At this point, for example, we can query if GPS is enabled by asking
// ifdef GPS_RX or GPS_TX

#else
#   error "Unrecognized board, check elibatt_config.h and change the TARGET_BOARD parameter."
#endif

#endif // __ELIBATT_CONFIG_H_
