package com.ckq.dasoftware;

/**
 * Created by Administrator on 2017/4/2.
 */

public class NativeMethod {

    static {
        System.loadLibrary("changle");
    }

    public native float nativeConverX2Fc(int x,int XN);
    public native float nativeConverY2Gain(int y,int YN,int MaxPdB,int MinNdB);
    public native int nativeConverFc2X(float f,int XN);
    public native int nativeConverGain2Y(float gain,int YN,int MaxPdB,int MinNdB);
    public native void nativeInPEQgraph(int ch,int[] x,int[] y, int XN, int YN,int MaxPdB,int MinNdB);
    public native void nativeUpdateInPEQ(int ch, int sec, int type,float[] Param);
    public native void nativeOutPEQgraph(int ch,int[] x,int[] y,int XN,int YN,int MaxPdB,int MinNdB);
    public native void nativeUpdateOutPEQ(int ch,int sec,int type,float[] Param);
    public native void nativeUpdateOutLpHp(int ch,int filterType,int filterAlgorithm,int filter_order, float fc);
    public native void nativeGlobalGraph(int ch,int[] x,int[] y,int XN,int YN,int MaxPdB,int MinNdB,float gain);
}
