#ifndef __UNIX_CMD_H_
#define __UNIX_CMD_H_

#include <Endpoint.h>
#include <SD.h>

#define FILENAME_MAX 32

class UnixCmd {
private:
    SerialEndpoint *out;

public:
    UnixCmd();
    ~UnixCmd();
    
    void setup(SerialEndpoint *dbg);

    void ls(bool removeIt=false);
    void mv(char *filename1, char *filename2);
    void rm(char *filename);
    void cat(char *filename);
    void touch(char *filename);

    int parseCmd(char *s, int len, char *f1, char *f2);

private:
    int readNextToken(char *s, int len, char *t, int &r, int max_t_len);
    bool equals(char *s, int len_s, char *t, int len_t);
    bool startsWith(char *s, int len_s, char *t, int len_t);
};

#endif // __UNIX_CMD_H_
