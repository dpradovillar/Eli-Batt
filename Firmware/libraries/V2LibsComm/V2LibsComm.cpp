#include <V2LibsComm.h>

V2LibsComm::V2LibsComm() :
    m_hard(NULL),
    m_soft(NULL),
    cmdLen(0),
    m_waiting_closing(false),
    m_last_command(CMD_INVALID)
{
}

V2LibsComm::~V2LibsComm() {
}

void V2LibsComm::setup(HardwareSerial *h, SoftwareSerial *s) {
    m_hard = h;
    m_soft = s;
}

void V2LibsComm::loop() {
    // Process hardware data only when software comm is finished
    if (!m_waiting_closing && m_hard->available()) {
        char c = m_hard->read();
        cmdBuffer[cmdLen++] = c;
        if (cmdLen >= BUFFER_MAX || c == '\n') {
            m_last_command = cmd.parseCmd(cmdBuffer, cmdLen);
            cmdLen = 0;
        }
    }
    if (m_soft->available()) {
        char c = m_soft->read();

        if (m_waiting_closing) {
            if (c == '>') {
                m_waiting_closing = false;
            }
            m_hard->write(c);
            if (c == '>') {
                m_hard->write('\n');
            }
        } else {
            if (c == '<') {
                m_waiting_closing = true;
                m_hard->write(c);
            } // else { throw away all bytes }
        }
    }
}

int V2LibsComm::getCommand() {
    return m_last_command;
}

int V2LibsComm::takeCommand() {
    int cmd = m_last_command;
    m_last_command = CMD_INVALID;
    return cmd;
}

