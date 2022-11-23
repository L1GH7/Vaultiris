package com.technologies.ate.settinghack;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MAIN";

    private EditText oldPass;
    private EditText newPass;
    private EditText userId;

    //http://androidxref.com/6.0.0_r5/xref/frameworks/base/core/java/com/android/internal/widget/ILockSettings.aidl#37
    private static final String SERVICE_CALL_LOCK_SETTING_HAVE_PASSWORD = "15";

    //http://androidxref.com/6.0.0_r5/xref/frameworks/base/core/java/com/android/internal/widget/ILockSettings.aidl#32
    private static final String SERVICE_CALL_LOCK_SETTING_CHANGE_PASSWORD = "10";



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String isPasswordExists = runCommand("service call lock_settings " + SERVICE_CALL_LOCK_SETTING_HAVE_PASSWORD + " i32 0");

        if (isPasswordExists.contains("1")){
            Log.d(TAG, "Device currently have password");
        }


    }

    public static String runCommand(String cmd) {
        System.out.println("Executing Shell CMD: " + cmd);
        String output = "";
        try {
            ProcessBuilder pb = new ProcessBuilder(cmd.split(" "));

            pb.redirectErrorStream(true);
            Process p = pb.start();
            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = in.readLine()) != null) {
                output += line;
            }
        } catch (IOException e) {
            e.printStackTrace();
            Log.w(TAG, "runCommand: ", e);
        }
        return output;
    }

    public void onBtnClicked(View view) {
        oldPass = (EditText) findViewById(R.id.old_pass);
        newPass = (EditText) findViewById(R.id.new_pass);
        userId = (EditText) findViewById(R.id.uid);
        Log.d(TAG, "onBtnClicked() uid" + userId.getText() + " old_pass " + oldPass.getText() + " new_pass " + newPass.getText());
        String result = runCommand(
                "service call lock_settings " + SERVICE_CALL_LOCK_SETTING_CHANGE_PASSWORD +
                        " s16 " + newPass.getText() +
                        " s16 " + oldPass.getText() +
                        " s16 " + userId.getText());

        Log.d(TAG, " result " + result);

    }
}
