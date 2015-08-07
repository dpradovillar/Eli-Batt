import cl.renetapia.servo.SerialServoController;
import cl.renetapia.servo.Ui;

import java.io.IOException;

public class Main implements Ui.Listener {
    private static final String mPortName = "COM4";

    private Ui mUi;
    private SerialServoController mController;

    public Main() throws IOException {
        mUi = new Ui();
        mUi.setListener(this);
        mUi.setVisible(true);

        mController = new SerialServoController(mPortName);
        if (!mController.open()) {
            throw new IOException("can't open port: " + mPortName);
        }
    }

    @Override
    public void onChanged(int pos1, int pos2) {
        try {
            byte b1 = (byte) pos1;
            byte b2 = (byte) pos2;
            mController.writePosition(b1, b2);
        } catch (IOException ioe) {
            throw new RuntimeException(ioe);
        }
    }

    public static void main(String[] args) throws IOException {
        new Main();
    }
}
