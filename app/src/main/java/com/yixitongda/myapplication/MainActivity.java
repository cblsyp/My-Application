package com.yixitongda.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends Activity {
    private static final String TAG = "chen debug";
    private static String CD_PATH= Environment.getExternalStorageDirectory().getAbsolutePath();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        diff();
        Log.d(TAG, "ClassLoader: " + this.getClassLoader().toString());
        Log.d(TAG,"system  libraryPath:"+System.getProperty("java.library.path"));
    }

    public void diff() {

        String path=CD_PATH+ File.separator+"Download"+File.separator+"android_video.mkv";
        Log.e(TAG,"path:"+path);
        String pattern_Path=CD_PATH+File.separator+"Download"+File.separator+"android_video_%d.mkv";
        Log.d(TAG, "diff: ");
        Fileutils.diff(path,pattern_Path, 4);
    }


}
