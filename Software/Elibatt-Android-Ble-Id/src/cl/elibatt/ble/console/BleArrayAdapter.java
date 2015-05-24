package cl.elibatt.ble.console;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.graphics.Typeface;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import cl.elibatt.ble.config.R;

import java.util.List;

public class BleArrayAdapter extends ArrayAdapter<BluetoothDevice> {

    private int mLayoutResourceId;
    private List<BluetoothDevice> mObjects;
    private int mSelectedPosition = -1;

    public boolean contains(String name) {
        if (name == null) {
            throw new NullPointerException("Name no debe ser null");
        }
        for (BluetoothDevice bt : mObjects) {
            if (name.equals(bt.getName())) {
                return true;
            }
        }
        return false;
    }

    public BleArrayAdapter(Context context, int layoutResourceId, List<BluetoothDevice> objects) {
        super(context, layoutResourceId, objects);

        mLayoutResourceId = layoutResourceId;
        mObjects = objects;
    }

    public void setSelected(int position) {
        mSelectedPosition = position;
        notifyDataSetInvalidated();
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View row = convertView;
        if(row == null) {
            LayoutInflater inflater = ((Activity)getContext()).getLayoutInflater();
            row = inflater.inflate(mLayoutResourceId, parent, false);
        }

        BluetoothDevice bd = mObjects.get(position);

        TextView bleName = (TextView) row.findViewById(R.id.ble_name);
        TextView bleAddress = (TextView) row.findViewById(R.id.ble_address);

        bleName.setTypeface(null, position == mSelectedPosition ? Typeface.BOLD :Typeface.NORMAL);
        bleAddress.setTypeface(null, position == mSelectedPosition ? Typeface.BOLD :Typeface.NORMAL);

        bleName.setText(bd.getName());
        bleAddress.setText(bd.getAddress());

        row.setTag(bd);

        return row;
    }
}
