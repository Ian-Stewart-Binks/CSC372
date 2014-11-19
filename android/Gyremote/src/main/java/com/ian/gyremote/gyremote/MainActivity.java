package com.ian.gyremote.gyremote;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.*;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.*;
import android.graphics.Color;

public class MainActivity extends Activity implements SensorEventListener {

    private Button something, forward, reverse, list, brake, right, left;
    private ImageView frontIndicator;
    private Set<BluetoothDevice> pairedDevices;
    private static final String TAG = "bluetooth";
    private SensorManager sManager;
    private TextView tv;
    public static final int MESSAGE_READ = 1;
    private Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            final int what = msg.what;
            switch(what) {
                case MESSAGE_READ: tv.setText(msg.toString()); break;
            }
        }
    };

    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;

    private InputStream inputStream = null;

    // Used to write to the bluetooth stream.
    private OutputStream outStream = null;

    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // MAC Address of bluetooth shield.
    private static String address = "00:6A:8E:16:C9:47";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        frontIndicator = (ImageView)findViewById(R.id.imageView3);
        something = (Button)findViewById(R.id.button1);
        forward = (Button)findViewById(R.id.button2);
        reverse = (Button)findViewById(R.id.button3);
        list = (Button)findViewById(R.id.button4);
        brake = (Button)findViewById(R.id.button5);
        right = (Button)findViewById(R.id.button6);
        left = (Button)findViewById(R.id.button);

        tv = (TextView)findViewById(R.id.textView3);

        btAdapter = BluetoothAdapter.getDefaultAdapter();


        new Thread (new Runnable() {
            @Override
            public void run() {
                final byte[] buffer = new byte[1024];
                int bytes; // bytes returned from read()

                while (true) {

                    try {
                        // Read from the InputStream
                        if (inputStream != null && inputStream.available() > 0) {
                            bytes = inputStream.read(buffer);
                            Log.d(TAG, buffer.toString());
                            mHandler.post(new Runnable()
                            {
                                public void run()
                                {
                                    try {

                                        String decoded = new String(buffer, "UTF-8");
//                                        tv.setText(decoded);
                                        if (decoded.charAt(0) == '0') {
                                            frontIndicator.setBackgroundColor(Color.BLUE);
                                        } else {
                                            frontIndicator.setBackgroundColor(Color.RED);
                                        }
                                    } catch (Exception e) {
                                        e.printStackTrace();
                                    }
                                }
                            });

                        }
                        // Send the obtained bytes to the UI activity
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    public void on(View view){
        if (!btAdapter.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
            Toast.makeText(getApplicationContext(),"Turned on", Toast.LENGTH_LONG).show();
        } else {
            Toast.makeText(getApplicationContext(),"Already on", Toast.LENGTH_LONG).show();
        }
    }

    public void list(View view){
        pairedDevices = btAdapter.getBondedDevices();

        ArrayList list = new ArrayList();
        for(BluetoothDevice dev : pairedDevices) {
            list.add(dev.getName());
            address = dev.getAddress();
        }

        Toast.makeText(getApplicationContext(),"Connected",
                Toast.LENGTH_SHORT).show();
    }

    public void off(View view){
        btAdapter.disable();
        Toast.makeText(getApplicationContext(),"Turned off" ,
                Toast.LENGTH_LONG).show();
    }

    public void readData() {

    }

    public void goForward(View view){
        sendData("g");
    }

    public void brake(View view){
        sendData("b");
    }

    public void reverse(View view) {
        sendData("r");
    }

    public void goLeft(View view) {
        sendData("l");
    }

    public void goRight(View view) {
        sendData("i");
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    public void run2() {
        byte[] buffer = new byte[1024];  // buffer store for the stream
        int bytes; // bytes returned from read()

        // Keep listening to the InputStream until an exception occurs
        while (true) {
            try {
                // Read from the InputStream
                if (inputStream != null) {
                    bytes = inputStream.read(buffer);
                    tv.setText(bytes+"");

                }
                // Send the obtained bytes to the UI activity
            } catch (IOException e) {
                break;
            }
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        // Set up connection to car.
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        try {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e) {
            e.printStackTrace();
        }

        btAdapter.cancelDiscovery();

        try {
            btSocket.connect();
        } catch (IOException e) {
            try {
                btSocket.close();
            } catch (IOException e2) {
                e2.printStackTrace();
            }
        }

        sManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sManager.registerListener(this,
                                  sManager.getDefaultSensor(Sensor.TYPE_ORIENTATION),
                                  SensorManager.SENSOR_DELAY_FASTEST);

        try {
            outStream = btSocket.getOutputStream();
            inputStream = btSocket.getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {

        try {
            final Method  m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", new Class[] { UUID.class });
            return (BluetoothSocket) m.invoke(device, MY_UUID);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return  device.createRfcommSocketToServiceRecord(MY_UUID);
    }

    @Override
    public void onPause() {
        super.onPause();

        if (outStream != null) {
            try {
                outStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        try     {
            btSocket.close();
        } catch (IOException e2) {
            e2.printStackTrace();
        }
    }

    private void sendData(String message) {
        if (message == currentChar) {
            return;
        }
        currentChar = message;
        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, address + "" + message);

        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    int currentRoll = 0;
    int currentPitch = 0;
    String currentChar = "";

    @Override
    public void onSensorChanged(SensorEvent event) {
//
//        // When on flat surface, values are:
//        // ROLL:   ~0 brake then -90 is 255
//        // PITCH:  ~0
//
//        // PROTOCOL:
//        // STRAIGHT:
//        // g: FORWARD FAST
//        // h: FORWARD SLOW
//        // r: REVERSE FAST
//        // p: REVERSE SLOW
//        // LEFT:
//        // a: FORWARD FAST
//        // e: FORWARD SLOW
//        // c: REVERSE FAST
//        // d: REVERSE SLOW
//        // RIGHT:
//        // u: FORWARD FAST
//        // x: FORWARD SLOW
//        // y: REVERSE FAST
//        // z: REVERSE SLOW
//        if (event.values[1] < 10 && event.values[1] > -10) {
//            if (event.values[2] > 25) { sendData("g"); } // Forward (fast)
//            else if (event.values[2] > 0) { sendData("h"); } // Forward (slow)
//            else if (event.values[2] < 0 && event.values[2] > -1) { sendData("b"); }  // Brake
//            else if (event.values[2] < -25) { sendData("r"); } // Reverse (fast)
//            else if (event.values[2] < 0) { sendData("p"); } // Reverse (slow)
//
//        } else if (event.values[1] < 25 && event.values[1] > 10) { // Left Slow
//            if (event.values[2] > 25) { sendData("a"); }  // Forward (fast)
//            else if (event.values[2] > 0) { sendData("e"); }  // Forward (slow)
//            else if (event.values[2] < 0 && event.values[2] > -1) { sendData("b"); } // Brake
//            else if (event.values[2] < -25) { sendData("c"); } // Reverse (fast)
//            else if (event.values[2] < 0) { sendData("d"); } // Reverse (slow)
//
//        } else if (event.values[1] > -25 && event.values[1] < -10) { // Right Slow
//            if (event.values[2] > 25) { sendData("u"); } // Forward (fast)
//            else if (event.values[2] > 0) { sendData("x"); }  // Forward (slow)
//            else if (event.values[2] < 0 && event.values[2] > -1) { sendData("b"); } // Brake
//            else if (event.values[2] < -25) { sendData("y"); } // Reverse (fast)
//            else if (event.values[2] < 0) { sendData("z"); } // Reverse (slow)
//
//        }
//        tv.setText("Orientation X (Roll) :"+ Float.toString(event.values[2]) +"\n"+
//                "Orientation Y (Pitch) :"+ Float.toString(event.values[1]) +"\n"+
//                "Orientation Z (Yaw) :"+ Float.toString(event.values[0]));
    }

    @Override
    public void onAccuracyChanged(Sensor arg0, int arg1) {
    }

}