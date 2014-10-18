package com.redbear.chat;

import java.util.*;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

public class Chat extends Activity {
    private final static String TAG = Chat.class.getSimpleName();

    public static final String EXTRAS_DEVICE = "EXTRAS_DEVICE";
    private BluetoothDevice device;

    private TextView tv = null;
    private EditText et = null;
    private Button btn = null;

    private String mDeviceName;
    private String mDeviceAddress;
    private RBLService mBluetoothLeService;
    private Map<UUID, BluetoothGattCharacteristic> map = new HashMap<UUID, BluetoothGattCharacteristic>();

    /*
    private GraphicalView mChart1;
    private XYMultipleSeriesDataset mDataset1 = new XYMultipleSeriesDataset();
    private XYMultipleSeriesRenderer mRenderer1 = new XYMultipleSeriesRenderer();
    private XYSeries mCurrentSeries1;
    private XYSeriesRenderer mCurrentRenderer1;

    private GraphicalView mChart2;
    private XYMultipleSeriesDataset mDataset2 = new XYMultipleSeriesDataset();
    private XYMultipleSeriesRenderer mRenderer2 = new XYMultipleSeriesRenderer();
    private XYSeries mCurrentSeries2;
    private XYSeriesRenderer mCurrentRenderer2;

    private GraphicalView mChart3;
    private XYMultipleSeriesDataset mDataset3 = new XYMultipleSeriesDataset();
    private XYMultipleSeriesRenderer mRenderer3 = new XYMultipleSeriesRenderer();
    private XYSeries mCurrentSeries3;
    private XYSeriesRenderer mCurrentRenderer3;

    private void initChart() {
        mCurrentSeries1 = new XYSeries("Voltaje");
        mDataset1.addSeries(mCurrentSeries1);
        mCurrentRenderer1 = new XYSeriesRenderer();
        mRenderer1.addSeriesRenderer(mCurrentRenderer1);
        mRenderer1.setLegendTextSize(48);

        mCurrentSeries2 = new XYSeries("Corriente");
        mDataset2.addSeries(mCurrentSeries2);
        mCurrentRenderer2 = new XYSeriesRenderer();
        mRenderer2.addSeriesRenderer(mCurrentRenderer2);
        mRenderer2.setLegendTextSize(48);

        mCurrentSeries3 = new XYSeries("Temperatura");
        mDataset3.addSeries(mCurrentSeries3);
        mCurrentRenderer3 = new XYSeriesRenderer();
        mRenderer3.addSeriesRenderer(mCurrentRenderer3);
        mRenderer3.setLegendTextSize(48);
    }
    */
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((RBLService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up
            // initialization.
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
                getGattService(mBluetoothLeService.getSupportedGattService());
            } else if (RBLService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getByteArrayExtra(RBLService.EXTRA_DATA));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.second);


        tv = (TextView) findViewById(R.id.textView);
        tv.setMovementMethod(ScrollingMovementMethod.getInstance());
        et = (EditText) findViewById(R.id.editText);
        btn = (Button) findViewById(R.id.send);
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                BluetoothGattCharacteristic characteristic = map.get(RBLService.UUID_BLE_SHIELD_TX);

                String str = et.getText().toString() + "\n";
                final byte[] tx = str.getBytes();

                displayData(tx);

                characteristic.setValue(tx);
                mBluetoothLeService.writeCharacteristic(characteristic);

                et.setText("");
            }
        });

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        device = bundle.getParcelable(EXTRAS_DEVICE);

        mDeviceAddress = device.getAddress();
        mDeviceName = device.getName();

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);

        Intent gattServiceIntent = new Intent(this, RBLService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
    }
/*
    private void addData(double v, double a, double t) {
        long time = System.currentTimeMillis();

        mCurrentSeries1.add(time, v);
        mCurrentSeries2.add(time, a);
        mCurrentSeries3.add(time, t);

        mChart1.repaint();
        mChart2.repaint();
        mChart3.repaint();
    }
*/
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
/*
    private List<byte[]> buffer = new ArrayList<byte[]>();

    private int sizeInBytes() {
        int count = 0;
        for (byte[]b : buffer) {
            count += b.length;
        }
        return count;
    }

    private byte[] toLinearArray() {
        int c = sizeInBytes();
        byte[] arr = new byte[c];
        int j = 0;
        for(byte[] b : buffer) {
            for(int i = 0; i < b.length; i++) {
                arr[j] = b[i];
                j++;
            }
        }
        return arr;
    }

*/
    /*
    private String strBuffer = "";
    private void processCharacter(char c) {
        if (c == '\n') {
            String[] parts = strBuffer.split(";");
            addData(Double.parseDouble(parts[0]), Double.parseDouble(parts[1]), Double.parseDouble(parts[2]));

            strBuffer = "";
        } else {
            strBuffer += c;
        }
    }*/

    private void displayData(byte[] byteArray) {
        if (byteArray != null) {
            // For handling binary message
            /*
            buffer.add(byteArray);
            if (sizeInBytes() >= 19) {
                byteArray = toLinearArray();
                buffer.clear();
            } else {
                return;
            }
            String data = "length=" + byteArray.length;// + ", type=" + (int)(byteArray[2]);
            */
            /*
            for (int i = 0; i < byteArray.length; i++) {
                processCharacter( (char)byteArray[i] );
            }
            */

            String data = "";
            for (int i = 0; i < byteArray.length; i++) {
                //processCharacter( (char)byteArray[i] );
                data += (char)byteArray[i];
            }


            tv.append(data);
            // find the amount we need to scroll. This works by
            // asking the TextView's internal layout for the position
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

    private void getGattService(BluetoothGattService gattService) {
        if (gattService == null)
            return;

        BluetoothGattCharacteristic characteristic = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_TX);
        map.put(characteristic.getUuid(), characteristic);

        BluetoothGattCharacteristic characteristicRx = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_RX);
        mBluetoothLeService.setCharacteristicNotification(characteristicRx, true);
        mBluetoothLeService.readCharacteristic(characteristicRx);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(RBLService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(RBLService.ACTION_DATA_AVAILABLE);

        return intentFilter;
    }
}
