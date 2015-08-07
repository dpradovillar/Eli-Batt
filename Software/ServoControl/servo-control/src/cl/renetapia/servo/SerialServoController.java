package cl.renetapia.servo;

import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.IOException;

public class SerialServoController {

    private final SerialPort serialPort;

    public SerialServoController(String portName) {
        serialPort = new SerialPort(portName);
    }

    public void writePosition(byte pos1, byte pos2) throws IOException {
        try {
            serialPort.writeByte(pos1);
            serialPort.writeByte(pos2);
        } catch (SerialPortException spe) {
            throw new IOException(spe.getMessage(), spe.getCause());
        }
    }

    public boolean open() throws IOException {
        try {
            return serialPort.openPort() && serialPort.setParams(9600, 8, 1, 0);
        } catch (SerialPortException spe) {
            throw new IOException(spe.getMessage(), spe.getCause());
        }
    }

    public boolean close() throws IOException {
        try {
            return serialPort.closePort();
        } catch (SerialPortException spe) {
            throw new IOException(spe.getMessage(), spe.getCause());
        }
    }
}
