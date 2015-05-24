package cl.elibatt.ble.console;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Handler;

import java.util.HashMap;
import java.util.Objects;

/**
 * Utilitary class for discovering new BLE devices.
 */
public class BleWrapper {

    private static final int REQUEST_ENABLE_BT = 1;

    public static final long SCAN_PERIOD = 3000;

    private BluetoothManager mBluetoothManager;
    private Context mContext;
    private BluetoothAdapter mBluetoothAdapter;

    public BleWrapper(Context context) {
        this(context, (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE));
    }

    public BleWrapper(Context context, BluetoothManager bluetoothManager) {
        mContext = context;
        mBluetoothManager = bluetoothManager;
    }

    /**
     * Interface for notifiyng events in the scanning of BLE devices process.
     */
    public interface DeviceListener {
        /**
         * The scanning has just started.
         */
        void onStarted();

        /**
         * Delivers a new BluetoothDevice detected.
         *
         * @param bd New BLE device detected. Guaranteed not to be one already notified since the onStarted() was
         *          called.
         */
        void onNewDevice(BluetoothDevice bd);

        /**
         * The scanning has just stopped.
         */
        void onFinished();
    }

    public boolean initAdapter() {
        return (mBluetoothAdapter = mBluetoothManager.getAdapter()) != null;
    }

    public boolean isEnabled() {
        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            return false;
        }
        return true;
    }

    public void enableBluetooth(Activity activity) {
        activity.startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), REQUEST_ENABLE_BT);
    }

    public boolean processResult(int requestCode, int resultCode) {
        // User chose not to enable Bluetooth.
        if (requestCode == REQUEST_ENABLE_BT && resultCode == Activity.RESULT_CANCELED) {
            return false;
        }
        return true;
    }

    /**
     * Runs an scanner for BLE devices, using the listener to deliver events for the scanning process.
     *
     * @param listener Object to be notified for events on the BLE devices scanner.
     */
    public void startScan(final DeviceListener listener) {
        final Handler handler = new Handler();
        new AsyncTask<Object, Object, Void>() {
            @Override
            protected Void doInBackground(Object... params) {
                if (listener != null) {
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            listener.onStarted();
                        }
                    });
                }

                final HashMap<String, BluetoothDevice> detected = new HashMap<String, BluetoothDevice>();

                if (mBluetoothAdapter != null) {
                    BluetoothAdapter.LeScanCallback callback = new BluetoothAdapter.LeScanCallback() {
                        @Override
                        public void onLeScan(final BluetoothDevice device, final int rssi, byte[] scanRecord) {
                            if (isValidDevice(device) && listener != null) {
                                handler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        if (!detected.containsKey(device.getAddress())) {
                                            detected.put(device.getAddress(), device);
                                            listener.onNewDevice(device);
                                        }
                                    }
                                });
                            }
                        }
                    };

                    mBluetoothAdapter.startLeScan(callback);

                    try {
                        Thread.sleep(SCAN_PERIOD);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    mBluetoothAdapter.stopLeScan(callback);
                }

                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        if (listener != null) {
                            listener.onFinished();
                        }
                    }
                });

                return null;
            }
        }.execute();
    }

    /**
     * Checks whether the BluetoothDevice is valid or not, based on business logic contained for Elibatt application.
     *
     * @param device Device
     * @return true if the device seems ok and this app should try to connect to it, otherwise it returns false.
     */
    private static boolean isValidDevice(BluetoothDevice device) {
        if (device == null) {
            return false;
        }
        String name = device.getName();
        if (name == null) {
            return false;
        }
        return true;
    }
}
