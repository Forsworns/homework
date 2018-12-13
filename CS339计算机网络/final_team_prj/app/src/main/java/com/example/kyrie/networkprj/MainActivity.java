package com.example.kyrie.networkprj;

import android.Manifest;
import android.content.pm.PackageManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.logging.LogRecord;

import static android.hardware.SensorManager.SENSOR_DELAY_NORMAL;

public class MainActivity extends AppCompatActivity {
    private SensorManager sensorManager;
    private Sensor accelerometer,gyroscope;
    private SensorEventListener accelerometerListener,gyroscopeListener;
    private static final int ACCELEROMETER_PERIOD = SENSOR_DELAY_NORMAL, GYROSCOPE_PERIOD = SENSOR_DELAY_NORMAL;
    private static final String ACCTAG = "Accelerometer test", GYROTAG = "Gyroscope test";
    private final int REQUESTCODE = 101;
    private static final String gyroTxt = "Gyroscope.txt", acceTxt = "Accelerometer.txt";

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main,menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()){
            case R.id.finish_item:
                Toast.makeText(this,"finish",Toast.LENGTH_SHORT).show();
                finish();
                android.os.Process.killProcess(android.os.Process.myPid());
                break;
            case R.id.delete_item:
                Toast.makeText(this,"清楚缓存",Toast.LENGTH_SHORT).show();
                delete(acceTxt);
                delete(gyroTxt);
            default:
        }
        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);

        int checkSelfPermission = checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if(checkSelfPermission == PackageManager.PERMISSION_DENIED){
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},REQUESTCODE);
        }

        Button buttonMS = (Button)findViewById(R.id.buttonMS);
        buttonMS.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,BluetoothSever.class);
                startActivity(intent);
            }
        });

        Button buttonWC = (Button)findViewById(R.id.buttonWC);
        buttonWC.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,BluetoothClient.class);
                startActivity(intent);
            }
        });

        // 传感器
        final TextView accelerometerTextView = (TextView) findViewById(R.id.accelerometer);
        final TextView gyroscopeTextView = (TextView) findViewById(R.id.gyroscope);

        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        gyroscope = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        if(accelerometer != null){
            accelerometerListener = new SensorEventListener() {
                @Override
                public void onSensorChanged(SensorEvent event) {
                    if(event.values.length!=0){
                        String textShown = "Accelerometer static:"+event.values[0]+","+event.values[1]+","+event.values[2];
                        Log.i(ACCTAG,textShown);
                        accelerometerTextView.setText(textShown);
                        save(acceTxt,event.values[0]+","+event.values[1]+","+event.values[2]+";");
                    }
                }

                @Override
                public void onAccuracyChanged(Sensor sensor, int accuracy) {
                    Log.i(ACCTAG,"Accuracy changed to: "+accuracy);
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
                        String textShown= "Gyroscope static:"+event.values[0]+","+event.values[1]+","+event.values[2];
                        Log.i(GYROTAG,textShown);
                        gyroscopeTextView.setText(textShown);
                        save(gyroTxt,event.values[0]+","+event.values[1]+","+event.values[2]+";");
                    }
                }

                @Override
                public void onAccuracyChanged(Sensor sensor, int accuracy) {
                    Log.i(GYROTAG,"Accuracy changed to: "+accuracy);
                }
            };
        }else{
            Toast.makeText(this,"陀螺仪不可用",Toast.LENGTH_SHORT).show();
        }

    }

    @Override
    protected void onResume(){
        super.onResume();
        sensorManager.registerListener(accelerometerListener,accelerometer,ACCELEROMETER_PERIOD);
        sensorManager.registerListener(gyroscopeListener,gyroscope,GYROSCOPE_PERIOD);
    }

    @Override
    protected  void onPause(){
        super.onPause();
        sensorManager.unregisterListener(accelerometerListener);
        sensorManager.unregisterListener(gyroscopeListener);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUESTCODE) {
            //询问用户权限
            if (permissions[0].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE) && grantResults[0]
                    == PackageManager.PERMISSION_GRANTED) {
                //用户同意
            } else {
                //用户不同意
            }
        }
    }

    public void save(String file, String input){
        File sdCardDir = Environment.getExternalStorageDirectory();
        File saveFile = new File(sdCardDir.getPath()+"/Android/data/",file);
        try{
            FileOutputStream out = new FileOutputStream(saveFile,true);
            out.write(input.getBytes());
            out.flush();
            out.close();
            Log.i("myFilter","successful");
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    public void delete(String fileName){
        File sdCardDir = Environment.getExternalStorageDirectory();
        String absolultePath = sdCardDir.getPath()+"/Android/data/" + fileName;
        File file2delete = new File(absolultePath);
        if(file2delete == null || !file2delete.exists()){
            return;
        }
        file2delete.delete();
    }
}
