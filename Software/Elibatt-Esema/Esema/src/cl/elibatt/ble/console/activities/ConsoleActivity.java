package cl.elibatt.ble.console.activities;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.*;
import android.graphics.Color;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.*;
import cl.elibatt.ble.R;
import cl.elibatt.ble.console.RBLService;
import cl.elibatt.ble.console.RBLUtils;
import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import java.util.*;

public class ConsoleActivity extends Activity {
    private final static String TAG = ConsoleActivity.class.getSimpleName();

    private BluetoothDevice mDevice;

    private String mDeviceName;
    private String mDeviceAddress;
    private RBLService mBluetoothLeService;
    private Map<UUID, BluetoothGattCharacteristic> map = new HashMap<UUID, BluetoothGattCharacteristic>();

    private static final int MAX_WINDOW_SIZE = 120;

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

    private static class Graph {
        private GraphicalView chart;
        private XYMultipleSeriesDataset dataset;
        private XYMultipleSeriesRenderer renderer;
        private XYSeries currentSeries;
        private XYSeriesRenderer currentRenderer;

        public Graph(Context context, int color, LinearLayout linerLayout, String graphTitle, String xAxisTitle, String yAxisTitle) {
            currentSeries = new XYSeries(yAxisTitle);
            currentRenderer = new XYSeriesRenderer();

            dataset = new XYMultipleSeriesDataset();
            dataset.addSeries(currentSeries);
            currentRenderer = new XYSeriesRenderer();
            renderer = new XYMultipleSeriesRenderer();
            renderer.addSeriesRenderer(currentRenderer);

            //float textSize = 24;

            //renderer.setLegendTextSize(textSize);
            //renderer.setAxisTitleTextSize(textSize);

            currentRenderer.setColor(color);
            currentRenderer.setPointStyle(PointStyle.DIAMOND);
            currentRenderer.setDisplayChartValues(true);
            currentRenderer.setLineWidth(2);
            currentRenderer.setFillPoints(true);

            renderer.setMarginsColor(Color.WHITE);
            renderer.setLabelsColor(Color.BLACK);
            renderer.setAxesColor(color);

            renderer.setChartTitle(graphTitle);
            renderer.setXTitle(xAxisTitle);
            renderer.setYTitle(yAxisTitle);


            renderer.setZoomEnabled(false);
            renderer.setPanEnabled(false);

            chart = ChartFactory.getLineChartView(context, dataset, renderer);

            linerLayout.addView(chart);
        }

        public void addData(long time, double value) {
            currentSeries.add(time, value);
        }

        public void repaint() {
            chart.repaint();
        }

        public void trim(int maxSize) {
            while (currentSeries.getItemCount() > maxSize) {
                currentSeries.remove(0);
            }
        }
    }

    private Graph graph1;
    private Graph graph2;
    private Graph graph3;

    private LinearLayout layout1;
    private LinearLayout layout2;
    private LinearLayout layout3;

    private TextView view1;
    private TextView view2;
    private TextView view3;

    private void initChart(LinearLayout layout1, LinearLayout layout2, LinearLayout layout3) {
        graph1 = new Graph(this, Color.rgb(153, 0, 0), layout1, "[V] vs [t]", "Tiempo", "Voltaje [V]");
        graph2 = new Graph(this, Color.rgb(0, 153, 0), layout2, "[A] vs [t]", "Tiempo", "Corriente [A]");
        graph3 = new Graph(this, Color.rgb(0, 0, 153), layout3, "[C] vs [t]", "Tiempo", "Temperatura [C]");
    }

    private void sendText(String text) {
        BluetoothGattCharacteristic characteristic = map.get(RBLService.UUID_BLE_SHIELD_TX);
        if (characteristic != null) {
            characteristic.setValue(text.getBytes());
            mBluetoothLeService.writeCharacteristic(characteristic);
        } else {
            Log.w(TAG, "Throwing characters away:" + text);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.console_layout);

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

        layout1 = (LinearLayout) findViewById(R.id.chart1);
        layout2 = (LinearLayout) findViewById(R.id.chart2);
        layout3 = (LinearLayout) findViewById(R.id.chart3);

        view1 = (TextView) findViewById(R.id.vars_temperature);
        view2 = (TextView) findViewById(R.id.vars_current);
        view3 = (TextView) findViewById(R.id.vars_voltage);

        initChart(layout1, layout2, layout3);

        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        sendText("S\n");
                    }
                });
            }
        }, 5000, 2000);

        findViewById(R.id.status_container).setVisibility(View.INVISIBLE);
        findViewById(R.id.status_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mLastLightIndicator) {
                    sendText("R0\n");
                } else {
                    sendText("R1\n");
                }
            }
        });
        sendText("R?\n");
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
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStop() {
        unregisterReceiver(mGattUpdateReceiver);

        super.onStop();
    }

    @Override
    protected void onDestroy() {
        mBluetoothLeService.disconnect();
        mBluetoothLeService.close();

        super.onDestroy();
    }

    private void displayData(byte[] byteArray) {
        if (byteArray != null) {
            for (int i = 0; i < byteArray.length; i++) {
                char c = (char) byteArray[i];
                if ((32 <= c && c <= 126) || c == '\t' || c == '\n') {
                    processChar(c);
                }
            }
        }
    }

    private StringBuilder sb = new StringBuilder();

    private void processChar(char c) {
        if (c == '\n') {
            String s = sb.toString();
            sb = new StringBuilder();
            processLine(s);
        } else {
            sb.append(c);
        }
    }

    private boolean mLastLightIndicator;

    private void setLightIndicator(boolean isOn) {
        findViewById(R.id.status_container).setVisibility(View.VISIBLE);
        ((TextView) findViewById(R.id.status_current)).setText(isOn ? "Encendido" : "Apagado");
        ((ImageView) findViewById(R.id.status_icon)).setImageResource(isOn ? R.drawable.bulb_on : R.drawable.bulb_off);
        ((Button) findViewById(R.id.status_button)).setText(isOn ? "Apagar" : "Encender");

        mLastLightIndicator = isOn;
    }

    private void processLine(String _response) {
        System.out.println("Processing line: (" + _response + ")");

        String response = _response.trim();
        if (response.startsWith("S:")) {
            String cleaned = response.substring(2);
            String []parts = cleaned.split(";");

            if (parts.length == 3) {
                try {
                    double t = Double.parseDouble(parts[0]);
                    double c = Double.parseDouble(parts[1]);
                    double v = Double.parseDouble(parts[2]);

                    addData(t, c, v);
                } catch (NumberFormatException nfe) {
                    nfe.printStackTrace();
                }
            }
        } else if (response.startsWith("R?")) {
            String cleaned = response.substring(2);
            if ("0".equals(cleaned)) {
                setLightIndicator(false);
            } else {
                setLightIndicator(false);
            }
        }
    }

    private void addData(double t, double c, double v) {
        long time = System.currentTimeMillis();

        graph1.addData(time, t);
        graph2.addData(time, c);
        graph3.addData(time, v);

        view1.setText(String.format("%.2f C", t));
        view2.setText(String.format("%.2f A", c));
        view3.setText(String.format("%.1f V", v));

        graph1.trim(MAX_WINDOW_SIZE);
        graph2.trim(MAX_WINDOW_SIZE);
        graph3.trim(MAX_WINDOW_SIZE);

        graph1.repaint();
        graph2.repaint();
        graph3.repaint();
    }
}
