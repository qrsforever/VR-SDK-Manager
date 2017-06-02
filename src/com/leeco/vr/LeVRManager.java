package com.leeco.vr;

import android.content.Context;
import android.util.Log;

public class LeVRManager
{
    private static final String TAG = "LeVRManager";
    static {
        System.loadLibrary("LeVRManager");
        native_init();
    }
    private static native final void native_init();
    public native int native_getViewOffset();
    public native int native_getHMDVersion();

    private static LeVRManager mVRMgr = null;
    private Context mContext = null;

    private LeVRManager(Context context) {
        mContext = context;
    }

    public static LeVRManager getInstance(Context context) {
        mVRMgr = new LeVRManager(context);
        return mVRMgr;
    }
}
