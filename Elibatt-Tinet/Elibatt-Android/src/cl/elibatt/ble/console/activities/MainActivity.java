package cl.elibatt.ble.console.activities;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;
import cl.elibatt.ble.console.BleWrapper;
import cl.elibatt.ble.console.R;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {

    public static final String EXTRAS_DEVICE = "EXTRAS_DEVICE";

    private Button connect;
    private Dialog mDialog;
    private Spinner mSpinner;

    private BleWrapper bleWrapper;

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

        mSpinner = (Spinner) findViewById(R.id.enter_mode);

        connect = (Button) findViewById(R.id.btn);
        connect.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                bleWrapper.startScan(MainActivity.this);
                showRoundProcessDialog(MainActivity.this, R.layout.loading_process_dialog_anim);
                Timer mTimer = new Timer();
                mTimer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        Bundle bundle = new Bundle();
                        bundle.putParcelable(MainActivity.EXTRAS_DEVICE, bleWrapper.getDevice());

                        Class<?> aClass;
                        String currentSelection = mSpinner.getSelectedItem().toString().toLowerCase().trim();

                        if ("console".equals(currentSelection)) {
                            aClass = ConsoleActivity.class;
                        } else if ("graph".equals(currentSelection)) {
                            aClass = GraphActivity.class;
                        } else {
                            throw new RuntimeException("Current spinner selection is not associated to any activity!");
                        }

                        startActivity(new Intent(getApplicationContext(), aClass).putExtras(bundle));
                        mDialog.dismiss();
                        MainActivity.this.finish();
                    }
                }, BleWrapper.SCAN_PERIOD);
            }
        });
    }

    public void showRoundProcessDialog(Context mContext, int layout) {
        mDialog = new AlertDialog.Builder(mContext).create();
        mDialog.setOnKeyListener(new OnKeyListener() {
            @Override
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_HOME || keyCode == KeyEvent.KEYCODE_SEARCH) {
                    return true;
                }
                return false;
            }
        });
        mDialog.show();
        mDialog.setContentView(layout);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!bleWrapper.isEnabled()) {
            bleWrapper.enableBluetooth(this);
        }
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
        this.finish();
    }
}
