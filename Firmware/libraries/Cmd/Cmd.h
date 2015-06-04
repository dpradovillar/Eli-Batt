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
#define CMD_ANALOG      8

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
