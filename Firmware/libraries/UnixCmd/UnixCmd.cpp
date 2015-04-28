#include <UnixCmd.h>

#define CMD_TEMPERATURE 1
#define CMD_CURRENT     2
#define CMD_VOLTAGE     3
#define CMD_GPS         4
#define CMD_LATITUDE    5
#define CMD_LONGITUDE   6
#define CMD_HEIGHT      7
#define CMD_LS          8
#define CMD_MV          9
#define CMD_RM          10
#define CMD_CAT         11
#define CMD_TOUCH       12

UnixCmd::UnixCmd() {
}

UnixCmd::~UnixCmd() {
}

void UnixCmd::setup(SerialEndpoint *endpoint) {
    out = endpoint;
}

void UnixCmd::ls(bool removeIt) {
  File root = SD.open("/");
  root.rewindDirectory();
  while(true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    if (removeIt) {
       if (!entry.isDirectory()) {
         out->print("Removing (");
         out->print(entry.name());
         out->print("): ");
       }
    } else {
      if (entry.isDirectory()) {
        out->print("D ");
      }else{
        out->print(entry.size());
        out->print(' ');
      }
      out->print(entry.name());
      out->println();
    }

    // If remove it, remove it
    if (removeIt) {
      out->println(SD.remove(entry.name()) ? "OK" : "ERROR");
    }
    
    entry.close();
  }
  root.close();
}

void UnixCmd::mv(char *filename1, char *filename2) {
  File from = SD.open(filename1);
  if (!from){
    out->print("Can't open file ");
    out->println(filename1);
    return;
  }
  
  File to = SD.open(filename2, FILE_WRITE);
  if(!to){
    out->print("Can't create file");
    out->println(filename2);
    
    from.close();
    return;
  }
  
  while(from.available()){
    char c = from.read();
    to.write(c);
  }
  from.close();
  to.close();
  SD.remove(filename1);
  out->println("File renamed OK!");
}

void UnixCmd::rm(char *filename) {
  if (equals(filename, 1, "*", 1)) {
    out->println("Removing everything!");
    ls(1);
    return;
  }  
  File dataFile = SD.open(filename);
  if (dataFile) {
    dataFile.close();
    out->println(SD.remove(filename) ? "File removed OK!" : "File couldn't be removed");
  } else {
    out->println("Can't remove a file that can't be open!");
  }
}

void UnixCmd::cat(char *filename) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available()) {
      out->write(dataFile.read());
      out->flush();
    }
    dataFile.close();
  } else {
    out->println("File is not available, maybe it doesn't exist?");
  }
}

void UnixCmd::touch(char *filename) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.close();
    out->println("File created OK!");
  } else {
    out->println("Can't create that file!");
  }
}

int UnixCmd::readNextToken(char *s, int len, char *t, int &r, int max_t_len) {
  r = 0;
  int i = 0;
  while(i<len && s[i] <= ' ') i++;
  
  if (i>=len) {
    t[0] = 0;
    return -1;
  }

  int j = 0;
  while(i<len && s[i] > ' ' && j < max_t_len) {
    t[j++] = s[i];
    i++;
  }
  r = j;
  return i;
}

bool UnixCmd::equals(char *s, int len_s, char *t, int len_t){
  if (len_s != len_t){
    return false;
  }
  for(int i=0; i<len_s; i++) {
    if (s[i] != t[i]){
      return false;
    }
  }
  return true;
}

bool UnixCmd::startsWith(char *s, int len_s, char *t, int len_t){
  if(len_t > len_s){
    return false;
  }
  return equals(s, len_t, t, len_t);
}

int UnixCmd::parseCmd(char *s, int len, char *f1, char *f2) {
  int ts, tl;
  
  if(startsWith(s, len, "T", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_TEMPERATURE;
  }
  
  if(startsWith(s, len, "C", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_CURRENT;
  }
  
  if(startsWith(s, len, "V", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_VOLTAGE;
  }
  
  if(startsWith(s, len, "GPS", 3)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_GPS;
  }
  
  if(startsWith(s, len, "LA", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_LATITUDE;
  }
  
  if(startsWith(s, len, "LO", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_LONGITUDE;
  }
  
  if(startsWith(s, len, "H", 1)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_HEIGHT;
  }
  
  if(startsWith(s, len, "ls", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return CMD_LS;
  }
  
  if(startsWith(s, len, "mv ", 3)){
    s += 3;
    len -= 3;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    s += skipped;
    len -= skipped;
    
    skipped = readNextToken(s, len, f2, r, FILENAME_MAX);
    if (r < 0) return -1;
    f2[r] = 0;

    return CMD_MV;
  }
  
  if (startsWith(s, len, "rm ", 3)){
    s += 3;
    len -= 3;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return CMD_RM;
  }
  
  if (startsWith(s, len, "cat ", 4)){
    s += 4;
    len -= 4;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return CMD_CAT;
  }
  
  if (startsWith(s, len, "touch ", 6)){
    s += 6;
    len -= 6;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return CMD_TOUCH;
  }
}

