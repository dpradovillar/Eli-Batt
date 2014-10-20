package cl.elibatt.ble.console.activities;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.*;
import android.os.Bundle;
import android.os.IBinder;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import cl.elibatt.ble.console.R;
import cl.elibatt.ble.console.RBLService;
import cl.elibatt.ble.console.RBLUtils;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class ConsoleActivity extends Activity {
    private final static String TAG = ConsoleActivity.class.getSimpleName();

    private BluetoothDevice mDevice;

    private TextView tv = null;
    private EditText et = null;
    private Button btn = null;

    private String mDeviceName;
    private String mDeviceAddress;
    private RBLService mBluetoothLeService;
    private Map<UUID, BluetoothGattCharacteristic> map = new HashMap<UUID, BluetoothGattCharacteristic>();

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((RBLService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if (RBLService.ACTION_GATT_DISCONNECTED.equals(action)) {
            } else if (RBLService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                RBLUtils.getGattService(mBluetoothLeService.getSupportedGattService(), mBluetoothLeService, map);
            } else if (RBLService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getByteArrayExtra(RBLService.EXTRA_DATA));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.console_layout);


        tv = (TextView) findViewById(R.id.textView);
        tv.setMovementMethod(ScrollingMovementMethod.getInstance());
        et = (EditText) findViewById(R.id.editText);
        btn = (Button) findViewById(R.id.send);
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                BluetoothGattCharacteristic characteristic = map.get(RBLService.UUID_BLE_SHIELD_TX);

                String str = et.getText().toString();

                if (characteristic != null) {

                    final byte[] tx = (str + "\n").getBytes();

                    displayData(tx);

                    characteristic.setValue(tx);
                    mBluetoothLeService.writeCharacteristic(characteristic);
                } else {
                    Log.w(TAG, "Throwing characters away:" + str);
                }

                et.setText("");
            }
        });

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        mDevice = (bundle == null ? null : (BluetoothDevice) bundle.getParcelable(MainActivity.EXTRAS_DEVICE));

        mDeviceAddress = (mDevice == null ? null : mDevice.getAddress());
        mDeviceName = (mDevice == null ? null : mDevice.getName());

        if (mDevice == null) {
            Toast.makeText(this, "No ble connected!", Toast.LENGTH_LONG).show();
        }

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);

        Intent gattServiceIntent = new Intent(this, RBLService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(mGattUpdateReceiver, RBLUtils.makeGattUpdateIntentFilter());
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            mBluetoothLeService.disconnect();
            mBluetoothLeService.close();

            System.exit(0);
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStop() {
        super.onStop();

        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mBluetoothLeService.disconnect();
        mBluetoothLeService.close();

        System.exit(0);
    }

    private void displayData(byte[] byteArray) {
        if (byteArray != null) {
            String data = "";
            for (int i = 0; i < byteArray.length; i++) {
                ;
                data += (char) byteArray[i];
            }

            tv.append(data);
            // find the amount we need to scroll. This works by asking the TextView's internal layout for the position
            // of the final line and then subtracting the TextView's height
            final int scrollAmount = tv.getLayout().getLineTop(tv.getLineCount()) - tv.getHeight();
            // if there is no need to scroll, scrollAmount will be <=0
            if (scrollAmount > 0) {
                tv.scrollTo(0, scrollAmount);
            } else {
                tv.scrollTo(0, 0);
            }
        }
    }
}
