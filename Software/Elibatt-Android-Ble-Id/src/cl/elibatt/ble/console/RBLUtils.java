package cl.elibatt.ble.console;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.IntentFilter;

import java.util.Map;
import java.util.UUID;

/**
 * Helper methods for interacting with RBLService.
 */
public class RBLUtils {

    /**
     * Prepares a BluetoothGattService for the operation RBLService will be responding to. A map will be popuplated with
     * with the writable characteristics (i.e. tx, and name change).
     *
     * @param gattService Entry point for BLE characteristics.
     * @param rblService RBLService for configuring characteristics.
     * @param map Mapping for available writable characteristics.
     */
    public static void getNameGattService(BluetoothGattService gattService, RBLService rblService,
            Map<UUID, BluetoothGattCharacteristic> map) {
        if (gattService == null) {
            return;
        }

        BluetoothGattCharacteristic shieldName = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_NAME);
        map.put(shieldName.getUuid(), shieldName);
        rblService.writeCharacteristic(shieldName);

        //BluetoothGattCharacteristic tx = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_TX);
        //map.put(tx.getUuid(), tx);

        //BluetoothGattCharacteristic rx = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_RX);
        //rblService.setCharacteristicNotification(rx, true);
        //rblService.readCharacteristic(rx);
    }

    /**
     * Creator for subscribers for the RBLService to get notifications of BLE device events.
     *
     * @return new Intent filter with all the actions for BLE device notifications already added.
     */
    public static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(RBLService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_SERVICES_DISCOVERED);
        //intentFilter.addAction(RBLService.ACTION_DATA_AVAILABLE);

        return intentFilter;
    }
}
