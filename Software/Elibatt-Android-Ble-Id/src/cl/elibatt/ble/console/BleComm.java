package cl.elibatt.ble.console;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.*;
import android.os.IBinder;
import android.text.TextUtils;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

/**
 * Communication logic between a generic controller and RBLService. All the events are passed on to another activity
 * through BleEventListener interface.
 */
public class BleComm {

    private static final String TAG = "BleComm";

    /**
     * Interface for notifying events to subscribers of BleComm.
     */
    public interface BleEventListener {
        /**
         * A new line hsa been detected. This method is independent of onNewData and it produces repeated data (onlye
         * one should be used, the one more appropriate to the application).
         *
         * @param bytes Bytes read
         * @param ascii
         */
        void onNewLine(byte []bytes, String ascii);

        /**
         *
         * @param bytes
         * @param ascii
         */
        void onNewData(byte []bytes, String ascii);

        /**
         * Triggered when this component is connected the first time to the RBLService.
         */
        void onConnected();

        /**
         * Triggered when this component is disconnected from the RBLService.
         */
        void onDisconnected();

        /**
         * Triggered after connected the first time, when it is able to communicate with the services of the BLE device.
         */
        void onServicesDiscovered();
    }

    private Activity mContext;
    private BleEventListener mListener;
    private RBLService mRBLService;
    private Map<UUID, BluetoothGattCharacteristic> mMap = new HashMap<UUID, BluetoothGattCharacteristic>();
    private BluetoothDevice mBluetoothDevice;
    private ByteArrayOutputStream mTempOutputArray;

    /**
     *
     * @param context For attaching to a RBLService.
     * @param listener For notifyng of new events.
     */
    public BleComm(Activity context, BleEventListener listener) {
        mContext = context;
        mListener = listener;
    }

    public void init() {
        Intent gattServiceIntent = new Intent(mContext, RBLService.class);
        mContext.bindService(gattServiceIntent, mServiceConnection, Activity.BIND_AUTO_CREATE);
    }

    /**
     * Makes the RBLService to communication with the provided device through the listener interface.
     *
     * @param bluetoothDevice Device to which connect. Only one device can be connected at a time.
     */
    public void connect(BluetoothDevice bluetoothDevice) {
        mBluetoothDevice = bluetoothDevice;
        mTempOutputArray = new ByteArrayOutputStream();

        // Automatically connects to the device upon successful start-up initialization.
        mRBLService.connect(mBluetoothDevice.getAddress());
    }

    /**
     * Send raw bytes to the BLE device.
     *
     * @param bytes Bytes to be sent by this method.
     */
    /*public void write(byte []bytes) {
        BluetoothGattCharacteristic charac = mMap.get(RBLService.UUID_BLE_SHIELD_TX);
        charac.setValue(bytes);
        mRBLService.writeCharacteristic(charac);
    }*/

    /**
     * Sends the bytes of an String and sends a new-line character \n (without the carriage return \r).
     *
     * @param text Text for which the bytes are going to be sent to the BLE device and followed by a new line.
     */
    /*public void println(String text) {
        write((text + "\n").getBytes());
    }*/

    /**
     * Sends the bytes of an String.
     *
     * @param text Text for which the bytes are going to be sent to the BLE device.
     */
    /*public void print(String text) {
        write(text.getBytes());
    }*/

    /**
     * Request the connected BLE device to change its connection name, thous, provoking a reset of the connection.
     *
     * @param newName non-empty, non-null, 20-length chars at most String for the name of the BLE connection to this
     *          device.
     */
    public void changeBleName(String newName) {
        if (TextUtils.isEmpty(newName)) {
            throw new IllegalArgumentException("Can't be empty!");
        }
        newName = newName.substring(0, Math.min(newName.length(), 20));
        BluetoothGattCharacteristic charac = mMap.get(RBLService.UUID_BLE_SHIELD_NAME);
        charac.setValue(newName.getBytes());
        mRBLService.writeCharacteristic(charac);
    }

    /**
     * Generic listeners for the connection of the RBLService. These are triggered from the Android Framework, when the
     * connection is stablished/closed between a component and a Service.
     */
    private final ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mRBLService = ((RBLService.LocalBinder) service).getService();
            if (!mRBLService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                mContext.finish();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mRBLService = null;
        }
    };

    /**
     * Enables the notification of events from RBLService into this component.
     */
    public void registerReceiver() {
        mContext.registerReceiver(mGattUpdateReceiver, RBLUtils.makeGattUpdateIntentFilter());
    }

    /**
     * Disables the notification of events from RBLService into this component.
     */
    public void unregisterReceiver() {
        mContext.unregisterReceiver(mGattUpdateReceiver);
    }

    /**
     * Requests the RBLService to cease.
     */
    public void disconnect() {
        if (mRBLService != null) {
            mRBLService.disconnect();
            mRBLService.close();
        }
    }

    /**
     * This BroadcastReceiver redirects the intent started by RBLService for new events on the BLE device into method
     * calls. This is convenient to have clean interfaces on the Activity side.
     */
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (RBLService.ACTION_GATT_DISCONNECTED.equals(action)) {
                onGattDisconnected();
            } else if (RBLService.ACTION_GATT_CONNECTED.equals(action)) {
                onGattConnected();
            } else if (RBLService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                onGattServicesDiscovered();
            //} else if (RBLService.ACTION_DATA_AVAILABLE.equals(action)) {
            //    onData(intent.getByteArrayExtra(RBLService.EXTRA_DATA));
            } else {
                onOther(action);
            }
        }
    };

    private void onGattDisconnected() {
        Log.v(TAG, "Action:" + RBLService.ACTION_GATT_DISCONNECTED);
        if (mListener != null) {
            mListener.onDisconnected();
        }
    }

    private void onGattConnected() {
        Log.v(TAG, "Action:" + RBLService.ACTION_GATT_SERVICES_DISCOVERED);
        if (mListener != null) {
            mListener.onConnected();
        }
    }

    private void onGattServicesDiscovered() {
        Log.v(TAG, "Action:" + RBLService.ACTION_GATT_SERVICES_DISCOVERED);
        RBLUtils.getNameGattService(mRBLService.getSupportedGattService(), mRBLService, mMap);
        if (mListener != null) {
            mListener.onServicesDiscovered();
        }
    }

    private void onByte(byte b) {
        if (b == 10) {
            byte []bytes = mTempOutputArray.toByteArray();
            if (mListener != null) {
                mListener.onNewLine(bytes, new String(bytes, Charset.forName("UTF-8")));
            }
            mTempOutputArray.reset();
        } else {
            mTempOutputArray.write(b);
        }
    }

    /*private void onData(byte []bytes) {
        Log.v(TAG, "Action:" + RBLService.ACTION_DATA_AVAILABLE);
        if (mListener != null) {
            mListener.onNewData(bytes, new String(bytes, Charset.forName("UTF-8")));
            for (int i=0; i<bytes.length; i++) {
                onByte(bytes[i]);
            }
        }
    }*/

    private void onOther(String action) {
        Log.w(TAG, "Action:" + action);
    }
}
