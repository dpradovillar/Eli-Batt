package cl.elibatt.ble.console;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.widget.ArrayAdapter;

import java.util.ArrayList;

/**
 * Created by rene on 18-10-14.
 */
public class BleWrapper {

    private static final int REQUEST_ENABLE_BT = 1;

    public static final long SCAN_PERIOD = 3000;

    private BluetoothManager mBluetoothManager;
    private Context mContext;

    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice mDevice;

    private String mPreferredConnectionName;

    public BleWrapper(Context context) {
        this(context, (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE));
    }

    public BleWrapper(Context context, BluetoothManager bluetoothManager) {
        mContext = context;
        mBluetoothManager = bluetoothManager;
    }

    public boolean initAdapter() {
        return (mBluetoothAdapter = mBluetoothManager.getAdapter()) != null;
    }

    public boolean acceptDevice(BluetoothDevice device) {
        if (isValidDevice(device)) {
            mDevice = device;
            return true;
        }
        return false;
    }

    public BluetoothDevice getDevice() {
        return mDevice;
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

    public void setPreferredConnectionName(String connectionName) {
        mPreferredConnectionName = connectionName;
    }

    public void scannDevices(final Activity activity, final ArrayAdapter<String> adapter) {
        new Thread() {
            @Override
            public void run() {
                activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        adapter.clear();
                    }
                });
                if (mBluetoothAdapter != null) {
                    final ArrayList<String> seenDevices = new ArrayList<String>();
                    BluetoothAdapter.LeScanCallback callback = new BluetoothAdapter.LeScanCallback() {
                        @Override
                        public void onLeScan(final BluetoothDevice device, final int rssi, byte[] scanRecord) {
                            activity.runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    String name = device.getName();
                                    if (!seenDevices.contains(name)) {
                                        adapter.add(name);
                                        seenDevices.add(name);
                                    }
                                }
                            });
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
            }
        }.start();
    }

    public void startScanAndAccept(final Activity activity) {
        new Thread() {
            @Override
            public void run() {
                if (mBluetoothAdapter != null) {
                    BluetoothAdapter.LeScanCallback callback = new BluetoothAdapter.LeScanCallback() {
                        @Override
                        public void onLeScan(final BluetoothDevice device, final int rssi, byte[] scanRecord) {
                            activity.runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    if (device != null && device.getName().equalsIgnoreCase(mPreferredConnectionName)) {
                                        acceptDevice(device);
                                    }
                                }
                            });
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
            }
        }.start();
    }

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
