package com.ckq.dasoftware.utils;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Build.VERSION;

/**
 * Created by Administrator on 2017/4/9.
 * 图片工具类
 */

public class ImageUtils {

    //drawable 转换成bitmap
    public static Bitmap drawableToBitmap(Drawable drawable) {

        if (drawable instanceof BitmapDrawable) {
            return ((BitmapDrawable) drawable).getBitmap();
        } else if (drawable instanceof BitmapDrawable) {
            Bitmap bitmap = Bitmap.createBitmap(       // 建立对应bitmap
                    drawable.getIntrinsicWidth(),     // 取drawable的长宽
                    drawable.getIntrinsicHeight(),
                    drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888 : Bitmap.Config.RGB_565);   //取drawable的颜色格式
            Canvas canvas = new Canvas(bitmap);  // 建立对应bitmap的画布
            drawable.setBounds(0, 0, drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight());
            drawable.draw(canvas);  // 把drawable内容画到画布中
            return bitmap;
        } else {
            return null;
        }
    }

    public static Bitmap createOverlapBitmap(Float ratio, Drawable background, Drawable front) {
        //Overlap:覆盖
        Bitmap bmp1 = drawableToBitmap(background);
        Bitmap bmp2 = drawableToBitmap(front);
        Canvas canvas = new Canvas(bmp1);
        int width = bmp2.getWidth();
        int height = bmp2.getHeight();
        int onLength = (int)((float)height * ratio.floatValue());
        canvas.drawBitmap(bmp2, new Rect(0, (height - onLength), width, height), new Rect(0, (height - onLength), width, height), null);
        return bmp1;
    }

    public static Bitmap createTextBitmap(String text, int textSize, int textColor, Drawable drawable, int drawableSize) {
        Bitmap bitmap = drawableToBitmap(drawable);
        bitmap = resizeBitmap(bitmap, ((float)drawableSize / (float)bitmap.getWidth()), ((float)drawableSize / (float)bitmap.getHeight()));
        Paint paint = new Paint(1);
        paint.setTextSize((float)textSize);
        paint.setColor(textColor);
        paint.setTextAlign(Paint.Align.CENTER);
        Canvas canvas = new Canvas(bitmap);
        canvas.drawText(text, (float)((bitmap.getWidth() - 2) / 2), (float)(((bitmap.getHeight() + textSize) - 6) / 2), paint);
        return bitmap;
    }


    public static Bitmap resizeBitmap(Bitmap bitmap, float widthScale, float heightScale) {

        Matrix matrix = new Matrix();
        matrix.postScale(widthScale, heightScale);
        Bitmap resizeBitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
        return resizeBitmap;
    }

    public static Drawable getDrawable(Context context, int resId) {
        if(Build.VERSION.SDK_INT >= 21) {    //系统的版本5.0以上时
          return context.getResources().getDrawable(resId,null);
        }
        return context.getResources().getDrawable(resId);
    }


    public static Drawable resizeDrawable(Context context, Drawable drawable, int w, int h) {
        int width = drawable.getIntrinsicWidth();
        int height = drawable.getIntrinsicHeight();
        Bitmap oldBmp = drawableToBitmap(drawable);
        float scaleWidth = (float)w / (float)width;
        float scaleHeight = (float)h / (float)height;
        Bitmap newBmp = resizeBitmap(oldBmp, scaleWidth, scaleHeight);
        return new BitmapDrawable(context.getResources(), newBmp);
    }


    public static Drawable resizeDrawable(Drawable drawable, int w, int h) {
        return resizeDrawable(null, drawable, w, h);
    }

}