package cl.renetapia.bluetooth.servocontrol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

class ConnectThread extends Thread {

    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;
    private final BluetoothAdapter mBluetoothAdapter;
    private final MyActivity mActivity;

    private Object mPositionMutex = new Object();
    private boolean mIsRunning = false;
    private int pos1ToSend = -1;
    private int pos2ToSend = -1;

    public ConnectThread(MyActivity activity, BluetoothAdapter bluetoothAdapter, BluetoothDevice device) {
        mBluetoothAdapter = bluetoothAdapter;
        mActivity = activity;

        // Use a temporary object that is later assigned to mmSocket,
        // because mmSocket is final
        BluetoothSocket tmp = null;
        mmDevice = device;

        // Get a BluetoothSocket to connect with the given BluetoothDevice
        try {
            // MY_UUID is the app's UUID string, also used by the server code
            tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            mBluetoothAdapter.cancelDiscovery();

        } catch (IOException e) {
            mActivity.postMessage(e.getMessage());
            Log.e(getClass().getSimpleName(), e.getMessage(), e);
        }
        mmSocket = tmp;
    }

    public void run() {
        // Cancel discovery because it will slow down the connection
        mBluetoothAdapter.cancelDiscovery();
        mIsRunning = true;
        OutputStream outputStream;
        try {
            // Connect the device through the socket. This will block
            // until it succeeds or throws an exception
            mmSocket.connect();
            outputStream = mmSocket.getOutputStream();
        } catch (IOException connectException) {
            // Unable to connect; close the socket and get out
            try {
                mmSocket.close();
            } catch (IOException ioe) {
                mActivity.postMessage(ioe.getMessage());
                Log.e(getClass().getSimpleName(), ioe.getMessage(), ioe);
            }
            return;
        }

        byte[] buffer = new byte[2];

        while (mIsRunning) {
            int pos1, pos2;
            synchronized (mPositionMutex) {
                pos1 = pos1ToSend;
                pos2 = pos2ToSend;
            }
            if (pos1 >= 0 && pos2 >= 0) {

                //Log.d(getClass().getSimpleName(), "Sending new positions pos1:" + pos1 + ", pos2:" + pos2);

                pos1ToSend = -1;
                pos2ToSend = -1;

                buffer[0] = (byte) pos1;
                buffer[1] = (byte) pos2;

                try {
                    outputStream.write(buffer, 0, 2);
                } catch (IOException ioe) {
                    mIsRunning = false;
                    mActivity.postMessage(ioe.getMessage());
                    Log.e(getClass().getSimpleName(), ioe.getMessage(), ioe);
                }
            }
        }
    }

    public void stopThread() {
        mIsRunning = false;
    }

    public void sendPosition(int pos1, int pos2) {
        synchronized (mPositionMutex) {
            pos1ToSend = pos1;
            pos2ToSend = pos2;
        }
    }

    /**
     * Will cancel an in-progress connection, and close the socket
     */
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            Log.e(getClass().getSimpleName(), e.getMessage(), e);
            mActivity.postMessage(e.getMessage());
        }
    }
}