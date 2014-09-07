#ifndef __SD_DATA_H_
#define __SD_DATA_H_

#include <DataStream.h>

class SdEndpointer;

/**
 * Wrapper for the built-in SD card library. It performs some application specific logic
 * as dumping messages as binary data, data compression, checksum verification and
 * specific file handling.
 * <p/>
 * 
 */
class SdData {
private:
  uint8_t m_chip_select_pin;

public:
  SdData();
  
  void setup(int chipSelectPin);
  
  bool open(const char *filename);
  
  bool print();
  
  bool close();
  
  void writeObject(DataObject *obj);
};

class SdEndpoint : public Endpoint {
public:
    ~SdEndpoint();
    
    void flush();
    int available();
    void waitForConnection();

    size_t write(byte b);
    int read();

    size_t write(byte *arr, size_t len);
    size_t read(byte *arr, size_t len);

    size_t write(byte *arr, size_t start, size_t len);
	size_t read(byte *arr, size_t start, size_t len);
	
    size_t print(int x);
    size_t print(char c);
    size_t print(const char *s);
    size_t print(char *s, int len);

    size_t println();
    size_t println(int x);
    size_t println(char x);
    size_t println(const char *s);
    size_t println(char *s, int len);
};

/**
 * Allows synchronized dumping of data
 *
 *
 *
 */
class BankData {

};

#endif // __SD_DATA_H_