package com.example.kyrie.networkprj;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class BluetoothSever extends AppCompatActivity {
    private List<String> bluetoothDevices = new ArrayList<String>();
    private BluetoothAdapter bluetoothAdapter;
    private ArrayAdapter<String> arrayAdapter;
    private final UUID MY_UUID = UUID.fromString("abcd1234-ab12-ab12-ab12-abcdef123456");

    private final String NAME = "Bluetooth_Socket";
    private BluetoothServerSocket serverSocket;
    private BluetoothSocket socket;
    private InputStream inputStream;
    private ServerThread serverThread;
    private TextView accelerometerTextView,gyroscopeTextView;

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Toast.makeText(getApplicationContext(), String.valueOf(msg.obj),
                    Toast.LENGTH_SHORT).show();
            accelerometerTextView.setText((String)msg.obj);
        }
    };

    private class ServerThread extends Thread{
        public ServerThread(){
            try{
                serverSocket = bluetoothAdapter.listenUsingRfcommWithServiceRecord(NAME,MY_UUID);
            }catch (Exception e){
                gyroscopeTextView.setText(e.getMessage());
            }
        }
        public void run(){
            try{
                socket = serverSocket.accept();
                inputStream = socket.getInputStream();
                while(true){
                    byte[] buffer = new byte[1024];
                    int count = inputStream.read();
                    Message msg = new Message();
                    msg.obj = new String(buffer,0,count,"utf-8");
                    handler.sendMessage(msg);
                }
            }catch (Exception e){
                gyroscopeTextView.setText(e.getMessage());
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),1);
        super.onCreate(savedInstanceState);
        accelerometerTextView = (TextView) findViewById(R.id.accelerometer);
        gyroscopeTextView = (TextView) findViewById(R.id.gyroscope);
        setContentView(R.layout.activity_bluetooth_sever);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        serverThread = new ServerThread();

        if(bluetoothAdapter == null){
            Toast.makeText(BluetoothSever.this,"不支持蓝牙",Toast.LENGTH_SHORT).show();
        }

        serverThread.start();
    }
}
