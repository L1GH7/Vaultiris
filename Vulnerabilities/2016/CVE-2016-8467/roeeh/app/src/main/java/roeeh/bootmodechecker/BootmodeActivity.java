package roeeh.bootmodechecker;

import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Field;

/*
        BootmodeChecker / Roee Hay (@roeehay)

        CVE-2016-8467 checker.
        BootmodeChecker detects whether your Nexus 6/6P device is vulnerable or patched, and if the bootmode system property
        has been tampered with, which may indicate that the device is under attack.

        More info about the vulnerability is available here:
        1. Blog post: https://securityintelligence.com/android-vulnerabilities-attacking-nexus-6-and-6p-custom-boot-modes/
        2. Another blog post: https://securityresear.ch/2017/01/05/attacking-android-custom-bootmodes/
        3. Paper: https://bit.ly/2iRwwUd

        Nexus 6P was patched as part of the January 2017 Security-bulletin
        Nexus 6 was patched as part of the November 2016 Security-bulletin
 */
public class BootmodeActivity extends AppCompatActivity {


    final static String GETPROP = "/system/bin/getprop";

    final static String LABEL_PATCHED = "PATCHED";
    final static String LABEL_VULN = "VULNERABLE";
    final static String LABEL_ATTACK = "BOOTMODE TAMPERED";

    final static int COLOR_RED = Color.rgb(0xcc,0x00,0x00);
    final static int COLOR_GREEN = Color.rgb(0x66,0x99,0);

    final static String PATCH_DATE_SHAMU  = "2016-11-05";
    final static String PATCH_DATE_ANGLER = "2017-01-05";
    final static String BLOG = "https://securityresear.ch/2017/01/05/attacking-android-custom-bootmodes/";

    final static String bootmode = getProperty("ro.boot.mode", "");
    final static boolean shamu = Build.DEVICE.equals("shamu");
    final static boolean angler = Build.DEVICE.equals("angler");
    final static String patch = getSecurityPatchLevel("unknown");
    final static String normalBootmode = "normal";


    final String TAG = this.getClass().getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button b = (Button)findViewById(R.id.button);

        TextView bootMode = (TextView) findViewById(R.id.textBootMode);
        bootMode.setText(bootmode);

        TextView codeName = (TextView) findViewById(R.id.textCodeName);
        codeName.setText(Build.DEVICE);

        TextView bootloader = (TextView) findViewById(R.id.textBootloader);
        bootloader.setText(Build.BOOTLOADER);


        TextView patchLevel = (TextView) findViewById(R.id.textPatchLevel);
        patchLevel.setText(patch);


        boolean isVuln = isVulnerable();
        TextView vuln = (TextView) findViewById(R.id.textVulnerable);
        if (shamu || angler) {
            vuln.setTextColor(isVuln ? COLOR_RED : COLOR_GREEN);
            b.setBackgroundColor(isVuln ? COLOR_RED : COLOR_GREEN);

            vuln.setText(isVuln ? LABEL_VULN : LABEL_PATCHED);
        }

        TextView bm = (TextView) findViewById(R.id.textBootmodeAttack);

        if (!"".equals(bootmode) && !normalBootmode.equals(bootmode))
        {
            bm.setText(LABEL_ATTACK);
        }

        vuln.setTextColor(isVulnerable() ? COLOR_RED : COLOR_GREEN);


        b.setOnClickListener(new View.OnClickListener() {
             @Override
             public void onClick(View view) {

                 Intent i = new Intent(Intent.ACTION_VIEW);
                 i.setData(Uri.parse(BLOG));
                 startActivity(i);
             }
         }
        );

    }

    private static boolean isVulnerable() {
        if (shamu)  return isPatchLevelBefore(PATCH_DATE_SHAMU);
        if (angler) return isPatchLevelBefore(PATCH_DATE_ANGLER);

        return false;
    }


    /* excepted date format: YYYY-MM-DD */
    private static boolean isPatchLevelBefore(String date) {
        String[] sDate = date.split("-");
        String[] sPatch = patch.split("-");

        assert (sDate.length == 3);
        if (sPatch.length != 3) return true;

        if (Integer.parseInt(sPatch[0]) < Integer.parseInt(sDate[0]))
            return true;

        if (Integer.parseInt(sPatch[0]) > Integer.parseInt(sDate[0]))
            return false;

        if (Integer.parseInt(sPatch[1]) < Integer.parseInt(sDate[1]))
            return true;

        if (Integer.parseInt(sPatch[1]) > Integer.parseInt(sDate[1]))
            return false;

        if (Integer.parseInt(sPatch[2]) < Integer.parseInt(sDate[2]))
            return true;

        return false;
    }

    private static String getSecurityPatchLevel(String def)
    {
        try {
            Field securityPatch = Build.VERSION.class.getField("SECURITY_PATCH");
            return (String)securityPatch.get(Build.VERSION.class);
        } catch (NoSuchFieldException e) {
            return def;
        } catch (IllegalAccessException e) {
            return def;
        }
    }
    private static String getProperty(String property, final String def) {
        Process process = null;
        try {
            process = new ProcessBuilder().command(GETPROP, property)
                    .redirectErrorStream(true).start();
        } catch (IOException e) {
            return def;
        }

        InputStream in = process.getInputStream();
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));

        try {
            return bufferedReader.readLine();
        } catch (IOException e) { }
        finally {
            process.destroy();
        }

        return def;
    }
}
