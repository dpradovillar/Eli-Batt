package cl.renetapia.bluetooth.servocontrol;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.*;

import java.util.HashSet;
import java.util.Set;

public class MyActivity extends Activity {

    private static final int REQUEST_ENABLE_BT = 1;

    private static <T> T find(int id, Activity root) {
        return (T) (root.findViewById(id));
    }

    private void showMessage(String message) {
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }

    public void postMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                showMessage(message);
            }
        });
    }

    private TextView mTitle1;
    private TextView mTitle2;
    private SeekBar mPosition1;
    private SeekBar mPosition2;
    private SeekBar.OnSeekBarChangeListener mListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            int i = (seekBar == mPosition1 ? 1 : 2);
            TextView target = (seekBar == mPosition1 ? mTitle1 : mTitle2);
            target.setText("Position of motor " + i + ": " + progress + "°");

            if (mConnectThread != null) {
                int pos1 = mPosition1.getProgress();
                int pos2 = mPosition2.getProgress();
                mConnectThread.sendPosition(pos1, pos2);

            }
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
        }
    };

    // For handling bluetooth state.
    private BluetoothAdapter mBluetoothAdapter;
    private Set<BluetoothDevice> mPairedDevices;
    private ListView mListView;
    private ArrayAdapter<String> mArrayAdapter;

    private Button mTurnButton;
    private Button mListButton;
    private Button mSearchButton;
    private Button mConnectButton;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mTitle1 = find(R.id.title1, this);
        mTitle2 = find(R.id.title2, this);

        mPosition1 = find(R.id.position1, this);
        mPosition2 = find(R.id.position2, this);

        mPosition1.setOnSeekBarChangeListener(mListener);
        mPosition2.setOnSeekBarChangeListener(mListener);

        // Bluetooth related stuff.
        mListView = find(R.id.discovered, this);
        mArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        mListView.setAdapter(mArrayAdapter);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            showMessage("Your device does not support Bluetooth");
        } else {
            mTurnButton = find(R.id.bluetooth_turn, this);
            mListButton = find(R.id.bluetooth_list, this);
            mSearchButton = find(R.id.bluetooth_search, this);
            mConnectButton = find(R.id.bluetooth_connect, this);

            updateTurnButton();
            mTurnButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (mBluetoothAdapter.isEnabled()) {
                        turnBluetooth(false);
                    } else {
                        turnBluetooth(true);
                    }
                    updateTurnButton();
                }
            });

            mListButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    listDevices();
                }
            });

            mSearchButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    discoverDevices();
                }
            });

            mConnectButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    connectToJyMcu();
                }
            });
        }
    }

    private void updateTurnButton() {
        if (mBluetoothAdapter.isEnabled()) {
            mTurnButton.setText("Turn bluetooth off");
        } else {
            mTurnButton.setText("Turn bluetooth on");
        }
    }

    private void turnBluetooth(boolean turnOn) {
        if (mBluetoothAdapter != null) {
            boolean isTurnedOn = mBluetoothAdapter.isEnabled();
            if (turnOn) {
                closeThreadedConnection();
                if (isTurnedOn) {
                    showMessage("Bluetooth already is turned on");
                } else {
                    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(intent, REQUEST_ENABLE_BT);
                    showMessage("Bluetooth turned on");
                }
            } else {
                if (isTurnedOn) {
                    showMessage("Bluetooth already is turned off");
                } else {
                    mBluetoothAdapter.disable();
                }
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mBroadcastReceiver);
        closeThreadedConnection();
    }

    private void closeThreadedConnection() {
        if (mConnectThread != null) {
            mConnectThread.stopThread();
            try {
                mConnectThread.join();
            } catch (InterruptedException e) {
                Log.e(getClass().getSimpleName(), e.getMessage(), e);
            }
            mConnectThread.cancel();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_BT) {
            if (mBluetoothAdapter.isEnabled()) {
                showMessage("Bluetooth enabled");
            } else {
                showMessage("Bluetooth disabled");
            }
        }
    }

    private void listDevices() {
        mPairedDevices = mBluetoothAdapter.getBondedDevices();
        mArrayAdapter.clear();
        for (BluetoothDevice device : mPairedDevices) {
            mArrayAdapter.add("BOND - " + device.getName() + "\n" + device.getAddress());
        }
        showMessage("Show paired devices");
    }

    final BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            if (mPairedDevices == null) {
                mPairedDevices = new HashSet<BluetoothDevice>();
            }

            String action = intent.getAction();
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                // add the name and the MAC address of the object to the arrayAdapter
                if (mPairedDevices.contains(device)) {
                    mArrayAdapter.add("BOND - " + device.getName() + "\n" + device.getAddress());
                } else {
                    mArrayAdapter.add("NEW - " + device.getName() + "\n" + device.getAddress());
                }
                mArrayAdapter.notifyDataSetChanged();
            }
        }
    };

    private void discoverDevices() {
        if (mBluetoothAdapter.isDiscovering()) {
            // the button is pressed when it discovers, so cancel the discovery
            mBluetoothAdapter.cancelDiscovery();
        } else {
            mArrayAdapter.clear();
            mBluetoothAdapter.startDiscovery();
            registerReceiver(mBroadcastReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        }
    }

    private ConnectThread mConnectThread;

    private void connectToJyMcu() {
        closeThreadedConnection();

        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice("20:14:05:14:10:55");
        mConnectThread = new ConnectThread(this, mBluetoothAdapter, device);
        mConnectThread.start();
    }

}
