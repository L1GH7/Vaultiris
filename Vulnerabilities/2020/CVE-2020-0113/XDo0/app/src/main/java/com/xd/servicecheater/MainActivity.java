package com.xd.servicecheater;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.security.Permission;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        checkPermission(Manifest.permission.ACCESS_FINE_LOCATION);

        checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        checkPermission(Manifest.permission.RECORD_AUDIO);
        Button button1 = findViewById(R.id.button1);
        Button button2 = findViewById(R.id.button2);
        Button button3 = findViewById(R.id.button3);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, MalServiceA.class);
                startForegroundService(intent);
            }
        });
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, MalServiceB.class);
                startForegroundService(intent);
            }
        });
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, NormalService.class);
                startForegroundService(intent);
            }
        });
    }

    private boolean checkPermission(String permission) {
        boolean permissionAccessApproved =
                ActivityCompat.checkSelfPermission(this,
                        permission) ==
                        PackageManager.PERMISSION_GRANTED;

        if (!permissionAccessApproved) {
            ActivityCompat.requestPermissions(this, new String[]{
                    permission
            }, 0);
        }
        return permissionAccessApproved;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {

        }
    }
}

