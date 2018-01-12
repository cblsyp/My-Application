package com.yixitongda.myapplication;

/**
 * Created by Administrator on 2017/12/6.
 */

public class Fileutils {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public static native void diff(String path,String patten_Path,int file_num);


}
