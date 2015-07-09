#ifndef __UNIX_CMD_H_
#define __UNIX_CMD_H_

#include <Endpoint.h>

#define FILENAME_MAX 32

#define CMD_NONE       -1 // A no-op command for doing nothing
#define CMD_INVALID     0 // For unrecognized commands

#define CMD_TEMPERATURE 1
#define CMD_CURRENT     2
#define CMD_VOLTAGE     3
#define CMD_GPS         4
#define CMD_LATITUDE    5
#define CMD_LONGITUDE   6
#define CMD_HEIGHT      7
#define CMD_SENSORS     8

#define CMD_AVG_TEMPERATURE   9
#define CMD_AVG_CURRENT      10
#define CMD_AVG_VOLTAGE      11

#define CMD_LIST_SIZE        12

#define CMD_LIST_TEMPERATURE 13
#define CMD_LIST_CURRENT     14
#define CMD_LIST_VOLTAGE     15

#define CMD_ID               16
#define CMD_LIST_ID          17

#define CMD_RELAY_ON         18
#define CMD_RELAY_OFF        19
#define CMD_RELAY_STATUS     20

class Cmd {
private:
    SerialEndpoint *out;

public:
    Cmd();
    ~Cmd();

    void setup(SerialEndpoint *dbg);
    int parseCmd(char *s, int len);

private:
    bool equals(char *s, int len_s, char *t, int len_t);
    bool startsWith(char *s, int len_s, char *t, int len_t);
};

#endif // __UNIX_CMD_H_
