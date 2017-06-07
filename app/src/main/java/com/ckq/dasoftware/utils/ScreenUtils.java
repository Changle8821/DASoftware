package com.ckq.dasoftware.utils;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;

/**
 * Created by Administrator on 2017/4/18.
 * 获得屏幕相关的辅助类
 */

public class ScreenUtils {

    private static final String TAG = "ScreenUtils";

    //根据手机的分辨率从 dp 的单位 转成为 px(像素)
    public static int dip2px(Context context, float dpValue) {   //dip: device independent pixels(设备独立像素)和设备硬件有关
        float scale = getScreenScale(context);
        return (int)((dpValue * scale) + 0.5f);    //+0.5是为了向上取整
    }

    //根据手机的分辨率从 px(像素) 的单位 转成为 dp
    public static int px2dip(Context context, float pxValue) {  // px: pixels(像素),这是绝对像素，是多少就永远是多少不会改变
        float scale = context.getResources().getDisplayMetrics().density;
        return (int)((pxValue / scale) + 0.5f);
    }

    public static int px2sp(Context context, float pxValue) {
        float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int)((pxValue / fontScale) + 0.5f);
    }

    public static int sp2px(Context context, float spValue) {   //sp: scaled pixels(放大像素)主要用于字体显示
        float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int)((spValue * fontScale) + 0.5f);
    }

    public static float getScreenScale(Context context) {
        return context.getResources().getDisplayMetrics().density;
    }

    public static float getScreenWidthRatio(Activity context) {
        return ((float)getScreenWidthInDip(context) / 865.0F);
    }

    public static float getScreenHeightRatio(Activity context) {
        return ((float)getScreenHeightInDip(context) / 310.0F);
    }

    public static int getLocalWidthFromDp(Activity context, float dpValue) {
        return Math.round(((float)dip2px(context, dpValue) * getScreenWidthRatio(context)));
    }

    public static int getLocalHeightFromDp(Activity context, float dpValue) {
        return Math.round(((float)dip2px(context, dpValue) * getScreenHeightRatio(context)));
    }

    public static int getLocalWidthFromPx(Activity context, int pxValue) {
        return Math.round(((float)pxValue * getScreenWidthRatio(context)));
    }

    public static int getLocalHeightFromPx(Activity context, int pxValue) {
        return Math.round(((float)pxValue * getScreenHeightRatio(context)));
    }

    public static int getScreenWidthInPixels(Activity context) {
        DisplayMetrics dm = new DisplayMetrics();
        context.getWindowManager().getDefaultDisplay().getMetrics(dm);
        return dm.widthPixels;
    }

    public static int getScreenHeightInPixels(Activity context) {
        DisplayMetrics dm = new DisplayMetrics();
        context.getWindowManager().getDefaultDisplay().getMetrics(dm);
        return dm.heightPixels;
    }

    public static int getScreenHeightInDip(Activity context) {
        DisplayMetrics dm = new DisplayMetrics();
        context.getWindowManager().getDefaultDisplay().getMetrics(dm);
        return px2dip(context, (float)dm.heightPixels);
    }

    public static int getScreenWidthInDip(Activity context) {
        DisplayMetrics dm = new DisplayMetrics();
        context.getWindowManager().getDefaultDisplay().getMetrics(dm);
        return px2dip(context, (float)dm.widthPixels);
    }

   //获得状态栏的高度

    public static int getStatusBarHeight(Activity activity) {
        int mStatusHeight = 0;
        int resourceId = activity.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if(resourceId > 0) {
            mStatusHeight = activity.getResources().getDimensionPixelSize(resourceId);
        }
        return mStatusHeight;
    }

    public static int getWindowHeight(Activity activity) {
        DisplayMetrics dm = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(dm);
        return dm.heightPixels;
    }

    public static int getScreenWidth(Context context) {
        return context.getResources().getDisplayMetrics().widthPixels;
    }
}
