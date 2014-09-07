package cl.renetapia.servo;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.io.IOException;

public class Ui extends JFrame {
    private static final int MIN_ANGLE = 0;
    private static final int MAX_ANGLE = 179;
    private static final int INIT_ANGLE = 90;

    private JSlider mSlider1;
    private JSlider mSlider2;
    private Listener mListener;
    private InternalJSliderListener mInternalListener;

    private JLabel servo1Label, servo2Label;

    public static interface Listener {
        public void onChanged(int pos1, int pos2);
    }

    private class InternalJSliderListener implements ChangeListener {
        @Override
        public void stateChanged(ChangeEvent e) {
            int pos1 = mSlider1.getValue();
            int pos2 = mSlider2.getValue();
            servo1Label.setText(pos1 + "°");
            servo2Label.setText(pos2 + "°");
            if (mListener != null) {
                mListener.onChanged(pos1, pos2);
            }
        }
    }

    public Ui() {
        setTitle("ServoControl");
        setLayout(new GridLayout(5, 1));
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setResizable(false);

        JPanel p1 = new JPanel(new GridLayout(1, 2));
        p1.add(new JLabel("Servo 1:"));
        p1.add(servo1Label = new JLabel(INIT_ANGLE + "°"));

        add(p1);
        add(mSlider1 = new JSlider(JSlider.HORIZONTAL, MIN_ANGLE, MAX_ANGLE, INIT_ANGLE));

        add(new JLabel("------------------------"), JLabel.CENTER_ALIGNMENT);

        JPanel p2 = new JPanel(new GridLayout(1, 2));
        p2.add(new JLabel("Servo 2:"));
        p2.add(servo2Label = new JLabel(INIT_ANGLE + "°"));
        add(p2);
        add(mSlider2 = new JSlider(JSlider.HORIZONTAL, MIN_ANGLE, MAX_ANGLE, INIT_ANGLE));

        mSlider1.setMajorTickSpacing(30);
        mSlider1.setMinorTickSpacing(1);
        mSlider1.setPaintTicks(true);
        mSlider1.setPaintLabels(true);

        mSlider2.setMajorTickSpacing(30);
        mSlider2.setMinorTickSpacing(1);
        mSlider2.setPaintTicks(true);
        mSlider2.setPaintLabels(true);

        mInternalListener = new InternalJSliderListener();

        mSlider1.addChangeListener(mInternalListener);
        mSlider2.addChangeListener(mInternalListener);

        pack();
    }

    public void setListener(Listener listener) {
        mListener = listener;
    }

}
