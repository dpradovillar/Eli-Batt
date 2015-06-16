#include <ParserComm.h>

int ParserComm::getIndexFor(uint32_t id) {
    for (int i = 0; i < slaves.n; i++) {
        if (slaves.id[i] == id) {
            return i;
        }
    }
    return -1;
}

ParserComm::ParserComm() :
    m_bank(NULL)
{
}

ParserComm::~ParserComm() {
    slaves.n = 0;
}

void ParserComm::setup(
#if USE_SOFT
        SoftwareSerial *s
#else
        HardwareSerial *s
#endif
) {
    m_bank = s;
}

void ParserComm::putMasterInfo(uint32_t id, float temp, float current, float voltage) {
    slaves.n = 1;
    slaves.id[0] = id;
    slaves.temperature[0] = temp;
    slaves.current[0] = current;
    slaves.voltage[0] = voltage;
}

void ParserComm::putSlaveInfo(uint32_t id, float temp, float current, float voltage) {
    int pos = getIndexFor(id);
    if (pos >= 0) {
        slaves.temperature[pos] = temp;
        slaves.current[pos] = current;
        slaves.voltage[pos] = voltage;
    } else if (slaves.n < MAX_SLAVES) {
        pos = slaves.n;
        slaves.id[pos] = id;
        slaves.temperature[pos] = temp;
        slaves.current[pos] = current;
        slaves.voltage[pos] = voltage;
        slaves.n++;
    } // else, don't store as there's no more room!
}

int toEntire(char *s, int len, int &length) {
    bool seenPoint = false;
    for (int i = 0; i < len; i++) {
        if (seenPoint) {
            s[]
        } else {

        }
    }
}

float getNumber(char *s, int len) {
    if (len == 0) {
        return 0f;
    }
    if (len == 1) {
        if ('0'<=s[0] && s[0]<='9') {
            return (float)(s[0]-'0');
        }
        return 0f;
    }
    int sign = +1;
    if (s[0] == '-') {
        sign = -1;
        s++;
        len--;
    }
    bool seenDecimal = false;
    int decimalPlaces = 0;
    for (int i = 0; i < len; i++) {

    }

}

void ParserComm::processLine(char *s, int len) {
    s[len] = 0;
    Serial.print("PROCESSING:[");
    Serial.print(s);
    Serial.print("], length=");
    Serial.println(len);
}

void ParserComm::loop() {
    static char buffer[MAX_LINE_BUFFER_SIZE];
    static int length = 0;

    if (m_bank->available()) {
        char u = m_bank->read();

        buffer[length++] = u;

        if (length >= MAX_LINE_BUFFER_SIZE || u == '\n' || u == '\r') {
            // procesar buffer,length

            // get rid of first characters
            char *ptr = buffer;
            while(length>0 && (*ptr<=' ' || *ptr=='<')) {
                ptr++;
                length--;
            }

            // get rid of last characters
            while(length>0 && (ptr[length-1]<=' ' || ptr[length-1]=='>')) {
                length--;
            }
            int sepCount = 0;
            // check the string is of the form "num1;num2;...;nump"
            bool valid = true;
            for (int i = 0; i < length; i++) {
                char c = ptr[i];
                bool isSep = (c==';');
                bool isOther = (c=='.' || c=='-');
                if (! ('0'<=c && c<='9') && !isSep && !isOther) {
                    valid = false;
                }
                if (isSep) {
                    sepCount++;
                }
            }
            if (length>0 && valid && sepCount==3) {
                processLine(ptr, length);
            }
            length = 0;
        }

    }
}
