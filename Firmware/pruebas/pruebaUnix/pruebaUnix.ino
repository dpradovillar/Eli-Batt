#include <SD.h>

#define FILENAME_MAX 32
#define BUFFER_MAX   100

/**
 * Utilitary sketch to list the SD card, dump, delete, rename and create files.
 * Please enter commands followed by a newline character. This program
 * will omit any space before, in the middle, or after any command or filename.
 *
 * The commands are as follows:
 * ls  : lists all the files in the root of the SD card.
 * mv <filename1> <filename2> : renames the file <filename1> and changes its name to <filename2>.
 * rm <filename> : deletes the file named <filename>.
 * cat <filename> : prints the content of the file named <filename>.
 * touch <filename> : creates empty file named <filename>
 *
 * Examples are given below:
 *
 * ls --> list root in the sd via serial console
 * touch a.txt --> creates empty file a.txt
 * mv a.txt b.txt --> renames a.txt into b.txt
 * mv FILE1 a.csv --> renames FILE1 into a.csv
 * rm a.csv  --> deletes a.csv from the sd
 * cat a.csv --> dumps a.csv via serial console
 */
const int chipSelect = 38;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }

  Serial.println("Try it out some UNIX-like commands: ls, mv, rm, cat, touch");
}

void ls(bool removeIt=false) {
  File root = SD.open("/");
  root.rewindDirectory();
  while(true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    if (removeIt) {
       if (!entry.isDirectory()) {
         Serial.print("Removing (");
         Serial.print(entry.name());
         Serial.print("): ");
       }
    } else {
      if (entry.isDirectory()) {
        Serial.print("D ");
      }else{
        Serial.print(entry.size());
        Serial.print(' ');
      }
      Serial.print(entry.name());
      Serial.println();
    }

    // If remove it, remove it
    if (removeIt) {
      Serial.println(SD.remove(entry.name()) ? "OK" : "ERROR");
    }
    
    entry.close();
  }
  root.close();
}

void mv(char *filename1, char *filename2) {
  File from = SD.open(filename1);
  if (!from){
    Serial.print("Can't open file ");
    Serial.println(filename1);
    return;
  }
  
  File to = SD.open(filename2, FILE_WRITE);
  if(!to){
    Serial.print("Can't create file");
    Serial.println(filename2);
    
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
  Serial.println("File renamed OK!");
}

void rm(char *filename) {
  
  if (equals(filename, 1, "*", 1)) {
    Serial.println("Removing everything!");
    ls(1);
    return;
  }  
  File dataFile = SD.open(filename);
  if (dataFile) {
    dataFile.close();
    Serial.println(SD.remove(filename) ? "File removed OK!" : "File couldn't be removed");
  } else {
    Serial.println("Can't remove a file that can't be open!");
  }
}

void cat(char *filename) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
      Serial.flush();
    }
    dataFile.close();
  } else {
    Serial.println("File is not available, maybe it doesn't exist?");
  }
}

void touch(char *filename) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.close();
    Serial.println("File created OK!");
  } else {
    Serial.println("Can't create that file!");
  }
}

int readNextToken(char *s, int len, char *t, int &r, int max_t_len) {
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

bool equals(char *s, int len_s, char *t, int len_t){
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

bool startsWith(char *s, int len_s, char *t, int len_t){
  if(len_t > len_s){
    return false;
  }
  return equals(s, len_t, t, len_t);
}

int parseCmd(char *s, int len, char *f1, char *f2) {
  int ts, tl;
  
  if(startsWith(s, len, "ls", 2)){
    f1[0] = 0;
    f2[0] = 0;
    return 0;
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

    return 1;
  }
  
  if (startsWith(s, len, "rm ", 3)){
    s += 3;
    len -= 3;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 2;
  }
  
  if (startsWith(s, len, "cat ", 4)){
    s += 4;
    len -= 4;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 3;
  }
  
  if (startsWith(s, len, "touch ", 6)){
    s += 6;
    len -= 6;
    int r;
    int skipped = readNextToken(s, len, f1, r, FILENAME_MAX);
    if (r < 0) return -1;
    f1[r] = 0;
    
    return 4;
  }
}

void loop() {
  static char cmdBuffer[BUFFER_MAX];
  static int cmdLen = 0;
  
  static char filename1[FILENAME_MAX+1];
  static char filename2[FILENAME_MAX+1];
  
  if (Serial.available()) {
    char c = Serial.read();
    cmdBuffer[cmdLen++] = c;
    Serial.write(c);
    
    if (cmdLen >= BUFFER_MAX || cmdBuffer[cmdLen-1] == '\n') {
        switch(parseCmd(cmdBuffer, cmdLen, filename1, filename2)){
          case 0:
            ls();
            Serial.println("##############################");
            break;
          case 1:
            mv(filename1, filename2);
            Serial.println("##############################");
            break;
          case 2:
            rm(filename1);
            Serial.println("##############################");
            break;
          case 3:
            cat(filename1);
            Serial.println("##############################");
            break;
          case 4:
            touch(filename1);
            Serial.println("##############################");
            break;
          default:
            Serial.println("Command not found!");
            Serial.println("##############################");
            break;
        }
        cmdLen = 0;
    }
  }
}

