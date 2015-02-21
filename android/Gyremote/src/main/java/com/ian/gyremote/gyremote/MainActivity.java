package com.ian.gyremote.gyremote;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.*;
import android.util.Log;
import android.view.Menu;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.view.View;
import android.widget.*;
import android.graphics.Color;

/**
 *
 */
public class MainActivity extends Activity implements SensorEventListener {

    /**
     *
     */
    private ImageView frontIndicator, backIndicator, leftIndicator, rightIndicator;

    /**
     *
     */
    private static final String TAG = "bluetooth";

    /**
     *
     */
    private SensorManager sManager;

    /**
     *
     */
    private TextView tv;

    /**
     *
     */
    private static final int MESSAGE_READ = 1;

    /**
     *
     */
    private Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            final int what = msg.what;
            switch(what) {
                case MESSAGE_READ: tv.setText(msg.toString()); break;
            }
        }
    };

    /**
     *
     */
    private BluetoothAdapter btAdapter = null;

    /**
     *
     */
    private BluetoothSocket btSocket = null;

    /**
     *
     */
    private InputStream inputStream = null;

    /**
     *
     */
    private OutputStream outStream = null;

    /**
     *
     */
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // MAC Address of bluetooth shield.
    private static String address = "00:6A:8E:16:C9:47";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        frontIndicator = (ImageView)findViewById(R.id.imageView3);
        rightIndicator = (ImageView)findViewById(R.id.imageView2);
        leftIndicator = (ImageView)findViewById(R.id.imageView4);
        backIndicator = (ImageView)findViewById(R.id.imageView);
        tv = (TextView)findViewById(R.id.textView);

        btAdapter = BluetoothAdapter.getDefaultAdapter();


        new Thread (new Runnable() {
            @Override
            public void run() {
                final byte[] buffer = new byte[1024];

                while (true) {
                    try {
                        if (inputStream != null && inputStream.available() > 0) {
                            mHandler.post(new Runnable() {
                                public void run() {
                                    try {
                                        String decoded = new String(buffer, "UTF-8");
                                        switch (decoded.charAt(0)) {
                                            case '0':
                                                frontIndicator.setBackgroundColor(Color.BLUE);
                                                break;
                                            case '1':
                                                frontIndicator.setBackgroundColor(Color.RED);
                                                break;
                                            case '2':
                                                backIndicator.setBackgroundColor(Color.BLUE);
                                                break;
                                            case '3':
                                                backIndicator.setBackgroundColor(Color.RED);
                                                break;
                                            case '4':
                                                rightIndicator.setBackgroundColor(Color.BLUE);
                                                break;
                                            case '5':
                                                rightIndicator.setBackgroundColor(Color.RED);
                                                break;
                                            case '6':
                                                leftIndicator.setBackgroundColor(Color.BLUE);
                                                break;
                                            case '7':
                                                leftIndicator.setBackgroundColor(Color.RED);
                                                break;
                                        }
                                    } catch (Exception e) {
                                        e.printStackTrace();
                                    }
                                }
                            });
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    /**
     *
     * @param view
     */
    public void on(View view) {
        sendData("A");
    }

    /**
     *
     * @param view
     */
    public void lightsOn(View view) {
        sendData("o");
    }

    /**
     *
     * @param view
     */
    public void disable(View view) {
        sendData("9");
    }

    /**
     *
     * @param view
     */
    public void list(View view) {
        Set<BluetoothDevice> pairedDevices;

        pairedDevices = btAdapter.getBondedDevices();

        ArrayList list = new ArrayList();
        for(BluetoothDevice dev : pairedDevices) {
            list.add(dev.getName());
            address = dev.getAddress();
        }

        Toast.makeText(getApplicationContext(),"Connected",
                Toast.LENGTH_SHORT).show();
    }

    /**
     *
     * @param view
     */
    void off(View view) {
        btAdapter.disable();
        Toast.makeText(getApplicationContext(),"Turned off" ,
                Toast.LENGTH_LONG).show();
    }

    /**
     *
     */
    void goForward(){
        sendData("g");
    }

    /**
     *
     * @param view
     */
    void brake(View view){
        sendData("b");
    }

    /**
     *
     */
    void reverse() {
        sendData("r");
    }

    /**
     *
     */
    void goLeft() {
        sendData("l");
    }

    /**
     *
     */
    void goRight() {
        sendData("i");
    }

    /**
     *
     * @param menu
     * @return
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
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

    /**
     *
     * @param device
     * @return
     * @throws IOException
     */
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

        try {
            btSocket.close();
        } catch (IOException e2) {
            e2.printStackTrace();
        }
    }

    /**
     *
     * @param message
     */
    private void sendData(String message) {
        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, address + "" + message);

        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        tv.setText("" + (event.values[2]));

        if (event.values[2] > 30) {
            goLeft();
        } else if (event.values[2] < -30) {
            goRight();
        } else if (event.values[1] > 10) {
            goForward();
        } else if (event.values[1] < -10) {
            reverse();
        }
    }

    @Override
    public void onAccuracyChanged(Sensor arg0, int arg1) {
    }

}
