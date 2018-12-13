package com.example.kyrie.networkprj;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import static android.hardware.SensorManager.SENSOR_DELAY_NORMAL;

public class BluetoothClient extends AppCompatActivity {
    private List<String> bluetoothDevices;
    private BluetoothAdapter bluetoothAdapter;
    private ArrayAdapter<String> arrayAdapter;
    private final UUID MY_UUID = UUID.fromString("abcd1234-ab12-ab12-ab12-abcdef123456");
    private ListView listView;

    // client
    private OutputStream outputStream;
    private BluetoothDevice device;
    private BluetoothSocket clientSocket;
    private BroadcastReceiver receiver;


    private SensorManager sensorManager;
    private Sensor accelerometer,gyroscope;
    private SensorEventListener accelerometerListener,gyroscopeListener;
    private static final int ACCELEROMETER_PERIOD = SENSOR_DELAY_NORMAL, GYROSCOPE_PERIOD = SENSOR_DELAY_NORMAL;

    @Override
    protected  void onPause(){
        super.onPause();
        sensorManager.unregisterListener(accelerometerListener);
        sensorManager.unregisterListener(gyroscopeListener);
    }

    @Override
    protected void onResume() {
        super.onResume();

        sensorManager.registerListener(accelerometerListener,accelerometer,ACCELEROMETER_PERIOD);
        sensorManager.registerListener(gyroscopeListener,gyroscope,GYROSCOPE_PERIOD);

        // 广播接收器
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        filter.setPriority(Integer.MAX_VALUE);
        this.registerReceiver(receiver,filter);
    }

    @Override
    protected void onStop() {
        super.onStop();
        this.unregisterReceiver(receiver);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_client);
        sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);

        bluetoothDevices = new ArrayList<String>();
        arrayAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,android.R.id.text1,bluetoothDevices);
        listView = (ListView) findViewById(R.id.devices);
        listView.setAdapter(arrayAdapter);

        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if(BluetoothDevice.ACTION_FOUND.equals(action)){
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    if(device.getBondState()!=BluetoothDevice.BOND_BONDED){
                        bluetoothDevices.add(device.getName() + ":"+ device.getAddress());
                        arrayAdapter.notifyDataSetChanged();//更新适配器
                    }
                }else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){}
            }
        };

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null){
            Toast.makeText(BluetoothClient.this,"不支持蓝牙",Toast.LENGTH_SHORT).show();
        }
        startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),1);

        Button buttonConnect = (Button)findViewById(R.id.buttonConnect);
        buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("myFilter","devices"+bluetoothDevices.size());
                bluetoothDevices.clear();
                Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();       // 已经配对的设备
                if(pairedDevices.size()>0){
                    for(BluetoothDevice device : pairedDevices){
                        bluetoothDevices.add(device.getName() + ":" + device.getAddress());
                        arrayAdapter.notifyDataSetChanged();
                    }
                }
                Toast.makeText(BluetoothClient.this,"正在搜索蓝牙设备",Toast.LENGTH_SHORT).show();
                if (bluetoothAdapter.isDiscovering()){
                    bluetoothAdapter.cancelDiscovery();
                }
                bluetoothAdapter.startDiscovery();
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String s = arrayAdapter.getItem(position);
                String address = s.substring(s.indexOf(":")+1).trim();
                Log.i("myFilter","have clicked"+String.valueOf(position));
                Toast.makeText(BluetoothClient.this,"click"+String.valueOf(position),Toast.LENGTH_SHORT).show();
                if(bluetoothAdapter.isDiscovering()){
                    bluetoothAdapter.cancelDiscovery();
                }
                if(device==null){
                    device = bluetoothAdapter.getRemoteDevice(address);
                }
                try{
                    if(clientSocket==null){
                        clientSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
                        clientSocket.connect();
                        outputStream = clientSocket.getOutputStream();
                    }
                    // 检测是否取到了outputstream
                    if(outputStream!=null){
                        final String textOutput = "Bluetooth!";
                        outputStream.write(textOutput.getBytes("utf-8"));
                        Toast.makeText(BluetoothClient.this,"已经选择发送给address",Toast.LENGTH_SHORT).show();
                    }else{
                        Toast.makeText(BluetoothClient.this,"未取到输出流",Toast.LENGTH_SHORT).show();
                    }
                } catch(Exception e){
                    Toast.makeText(BluetoothClient.this,"连接错误"+e.getMessage(),Toast.LENGTH_SHORT).show();
                }
            }
        });

        final TextView gyroscopeTextView = (TextView) findViewById(R.id.tempStore);

        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        gyroscope = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        if(accelerometer != null){
            accelerometerListener = new SensorEventListener() {
                @Override
                public void onSensorChanged(SensorEvent event) {
                    if(event.values.length!=0){
                        String textShown = +event.values[0]+","+event.values[1]+","+event.values[2]+";";
                        textShown += gyroscopeTextView.getText();
                        if(outputStream!=null){
                            try{
                                outputStream.write(textShown.getBytes("utf-8"));
                            }catch (IOException e){
                            }
                        }
                    }
                }

                @Override
                public void onAccuracyChanged(Sensor sensor, int accuracy) {
                }
            };
        }else{
            Toast.makeText(this,"加速度传感器不可用",Toast.LENGTH_SHORT).show();
        }


        if(gyroscope != null){
            gyroscopeListener = new SensorEventListener() {
                @Override
                public void onSensorChanged(SensorEvent event) {
                    if(event.values.length!=0){
                        String textShown= +event.values[0]+","+event.values[1]+","+event.values[2]+";";
                        gyroscopeTextView.setText(textShown);
                    }
                }

                @Override
                public void onAccuracyChanged(Sensor sensor, int accuracy) {
                }
            };
        }else{
            Toast.makeText(this,"陀螺仪不可用",Toast.LENGTH_SHORT).show();
        }

    }
}
