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
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import cl.elibatt.ble.console.R;
import cl.elibatt.ble.console.RBLService;
import cl.elibatt.ble.console.RBLUtils;
import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class GraphActivity extends Activity {
    private final static String TAG = GraphActivity.class.getSimpleName();

    private BluetoothDevice mDevice;

    private String mDeviceName;
    private String mDeviceAddress;
    private RBLService mBluetoothLeService;
    private Map<UUID, BluetoothGattCharacteristic> map = new HashMap<UUID, BluetoothGattCharacteristic>();


    private static final int MAX_WINDOW_SIZE = 120;

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

            float textSize = 24;

            renderer.setLegendTextSize(textSize);
            renderer.setAxisTitleTextSize(textSize);

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
        setContentView(R.layout.graph_layout);

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

        view1 = (TextView) findViewById(R.id.view_voltage);
        view2 = (TextView) findViewById(R.id.view_amps);
        view3 = (TextView) findViewById(R.id.view_temperature);

        initChart(layout1, layout2, layout3);
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (graph1 == null || graph1.chart == null) {
            initChart(layout1, layout2, layout3);
        }

        registerReceiver(mGattUpdateReceiver, RBLUtils.makeGattUpdateIntentFilter());
    }

    private void addData(double v, double a, double t) {
        long time = System.currentTimeMillis();

        graph1.addData(time, v);
        graph2.addData(time, a);
        graph3.addData(time, t);

        view1.setText(String.format("%.2f [V]", v));
        view2.setText(String.format("%.2f [A]", a));
        view3.setText(String.format("%.1f [C]", t));

        graph1.trim(MAX_WINDOW_SIZE);
        graph2.trim(MAX_WINDOW_SIZE);
        graph3.trim(MAX_WINDOW_SIZE);

        graph1.repaint();
        graph2.repaint();
        graph3.repaint();
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

    private StringBuilder buffer = new StringBuilder();

    private void onNewLine(String line) {
        String trimmed = line.trim().replace(';', ',').replace('\t', ',');
        String[] parts = trimmed.split(",");
        if (parts.length >= 3) {
            addData(
                    Double.parseDouble(parts[0]),
                    Double.parseDouble(parts[1]),
                    Double.parseDouble(parts[2])
            );
        } else {
            Log.w(TAG, "Warning, line received should contain 3 or more numeric values");
        }
    }

    private void processCharacter(char c) {
        if (c == '\n') {
            onNewLine(buffer.toString());
            buffer = new StringBuilder();
        } else {
            buffer.append(c);
        }
    }

    private void displayData(byte[] byteArray) {
        if (byteArray != null) {
            for (int i = 0; i < byteArray.length; i++) {
                processCharacter((char) byteArray[i]);
            }
        }
    }

}
