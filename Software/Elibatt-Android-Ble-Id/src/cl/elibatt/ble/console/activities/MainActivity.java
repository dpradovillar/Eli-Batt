package cl.elibatt.ble.console.activities;

import android.app.Activity;
import android.bluetooth.*;
import android.content.*;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.Toast;
import cl.elibatt.ble.config.R;
import cl.elibatt.ble.console.*;

import java.util.*;

public class MainActivity extends Activity implements BleWrapper.DeviceListener, BleComm.BleEventListener {

    private static final String TAG = "BleComm.MainActivity";

    private BleWrapper bleWrapper;

    private RelativeLayout mDetailsContainer;
    private Button mRefresh;
    private ProgressBar mProgressBar;
    private EditText mBleNameInput;
    private BleArrayAdapter mBtArrayAdapter;
    private BluetoothDevice mLastSelectedDevice;
    private BleComm mBleComm;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);

        setContentView(R.layout.main_layout);

        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "Ble not supported", Toast.LENGTH_SHORT).show();
            finish();
        }

        bleWrapper = new BleWrapper(this);

        if (!bleWrapper.initAdapter()) {
            Toast.makeText(this, "Ble not supported", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        mBtArrayAdapter = new BleArrayAdapter(this, R.layout.listview_item_row, new ArrayList<BluetoothDevice>());
        mBtArrayAdapter.clear();

        mBtArrayAdapter.notifyDataSetChanged();

        mRefresh = byId(R.id.refresh_button);
        mDetailsContainer = byId(R.id.details_container);
        mProgressBar = byId(R.id.progress_bar);
        Button bleSave = byId(R.id.ble_name_save);
        mBleNameInput = byId(R.id.ble_name_input);
        ListView devicesList = byId(R.id.devices_list);

        mDetailsContainer.setVisibility(View.GONE);

        devicesList.setAdapter(mBtArrayAdapter);
        devicesList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                mLastSelectedDevice = mBtArrayAdapter.getItem(position);
                mDetailsContainer.setVisibility(View.VISIBLE);
                mBleNameInput.setText(mLastSelectedDevice.getName());
                mBtArrayAdapter.setSelected(position);
            }
        });

        mRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mBleComm.disconnect();

                mLastSelectedDevice = null;
                mBtArrayAdapter.clear();
                bleWrapper.startScan(MainActivity.this);
            }
        });

        bleSave.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mLastSelectedDevice != null) {

                    String bleName = mBleNameInput.getText().toString();

                    if (TextUtils.isEmpty(bleName)) {
                        showError("Nombre de BLE vacio es invalido");
                    } else if (bleName.length() > 20) {
                        showError("Nombre de BLE muy largo: " + bleName.length());
                    } else if (bleName.equals(mLastSelectedDevice.getName())) {
                        showError("Debe cambiar el nombre/id");
                    } else if (mBtArrayAdapter.contains(bleName)) {
                        showError("Nombre ocupado, utilice otro");
                    }
                    // Todo OK!
                    else {
                        mBleComm.connect(mLastSelectedDevice);
                    }
                }
            }
        });

        mBleComm = new BleComm(this, this);
        mBleComm.init();
    }

    void showError(String message) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!bleWrapper.isEnabled()) {
            bleWrapper.enableBluetooth(this);
        }
        mBleComm.registerReceiver();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (!bleWrapper.processResult(requestCode, resultCode)) {
            finish();
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mBleComm.unregisterReceiver();
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mBleComm.disconnect();
    }

    private <T> T byId(int id) {
        return (T) findViewById(id);
    }

    @Override
    public void onStarted() {
        mBtArrayAdapter.clear();
        mBtArrayAdapter.setSelected(-1);
        mRefresh.setEnabled(false);
        mProgressBar.setVisibility(View.VISIBLE);
        mDetailsContainer.setVisibility(View.GONE);
    }

    @Override
    public void onNewDevice(BluetoothDevice bd) {
        mBtArrayAdapter.add(bd);
    }

    @Override
    public void onFinished() {
        mRefresh.setEnabled(true);
        mProgressBar.setVisibility(View.INVISIBLE);
    }

    @Override
    public void onNewLine(byte[] bytes, String ascii) {
        Log.v(TAG, "onNewLine:" + ascii);
    }

    @Override
    public void onNewData(byte[] bytes, String ascii) {
    }

    @Override
    public void onConnected() {
        Log.v(TAG, "onConnected");
    }

    @Override
    public void onDisconnected() {
        Log.v(TAG, "onDisconnected");
    }

    @Override
    public void onServicesDiscovered() {
        Log.v(TAG, "onServicesDiscovered");
        String bleName = mBleNameInput.getText().toString();
        mBleComm.changeBleName(bleName);

        mDetailsContainer.setVisibility(View.GONE);
    }
}
