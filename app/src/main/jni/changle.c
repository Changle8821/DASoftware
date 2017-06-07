#include <jni.h>
#include "GraphDSP.h"


/***************************GEQ画图参数*********************************/
#define  GEQ_XN              944	         // GEQ x轴的点数
#define  GEQ_YN              165	         // GEQ y轴的点数
#define  MaxGEQdB            15	             // GEQ y轴的最大dB值
#define  MinGEQdB            -30	         // GEQ y轴的最小dB值
/***********************************************************************/

/****************************IN PEQ参数*********************************/
#define  IN_PEQ_XN           865	         // IN PEQ x轴的点数
#define  IN_PEQ_YN           310	         // IN PEQ y轴的点数
#define  MaxInPEQdB          20	             // GEQ y轴的最大dB值
#define  MinInPEQdB          -40	         // GEQ y轴的最小dB值
/***********************************************************************/

/***************************OUT PEQ参数*********************************/
#define  OUT_PEQ_XN          865	         // OUT PEQ x轴的点数
#define  OUT_PEQ_YN          310	         // OUT PEQ y轴的点数
#define  MaxOutPEQdB         20	             // GEQ y轴的最大dB值
#define  MinOutPEQdB         -40	         // GEQ y轴的最小dB值

/****************************GLOBAL参数*********************************/
#define  GLOBAL_XN           940	         // GLOBAL x轴的点数
#define  GLOBAL_YN           400	         // GLOBAL y轴的点数
#define  MaxGLOBALdB         20              // GEQ y轴的最大dB值
#define  MinGLOBALdB         -40	         // GEQ y轴的最小dB值
/***********************************************************************/


#define  IN_PEQ_PARAM_NUM    3	             // IN PEQ 的参数个数
#define  OUT_PEQ_PARAM_NUM   3	             // OUT PEQ 的参数个数

// PEQ的几种类型
#define peak                 0
#define low_pass             1
#define high_pass            2
#define low_shelf            3
#define high_shelf           4
#define all_pass             5

// Out里面的Highpass Filter、 Lowpass Filter的算法类型
#define NOALOGRITHM          0
#define BUTTERWORTH		     1
#define LINKRILEY            2
#define BESSEL               3

#define divFreqLOW           0               //低通滤波器
#define divFreqHIGH          1               //高通滤波器





JNIEXPORT jfloat JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeConverX2Fc(JNIEnv *env, jobject instance, jint x,
                                                      jint XN) {
    // TODO
    return ConverX2Fc(x,XN);

}


JNIEXPORT jfloat JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeConverY2Gain(JNIEnv *env, jobject instance, jint y,
                                                        jint YN, jint MaxPdB, jint MinNdB) {

    // TODO
    return ConverY2Gain(y,YN,MaxPdB,MinNdB);

}

JNIEXPORT jint JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeConverFc2X(JNIEnv *env, jobject instance, jfloat f,
                                                      jint XN) {

    // TODO
    return ConverFc2X(f,XN);

}

JNIEXPORT jint JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeConverGain2Y(JNIEnv *env, jobject instance, jfloat gain,
                                                        jint YN, jint MaxPdB, jint MinNdB) {

    // TODO
    return ConverGain2Y(gain,YN,MaxPdB,MinNdB);

}


JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeUpdateInPEQ(JNIEnv *env, jobject instance, jint ch,
                                                       jint sec, jint type, jfloatArray Param_) {
    jfloat *Param = (*env)->GetFloatArrayElements(env, Param_, NULL);

    // TODO
    UpdateInPEQ(ch,sec, type, Param);
    (*env)->ReleaseFloatArrayElements(env, Param_, Param, 0);
}

JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeInPEQgraph(JNIEnv *env, jobject instance, jint ch,
                                                      jintArray x_, jintArray y_, jint XN, jint YN,
                                                      jint MaxPdB, jint MinNdB) {
    jint *x = (*env)->GetIntArrayElements(env, x_, NULL);
    jint *y = (*env)->GetIntArrayElements(env, y_, NULL);

    // TODO
    InPEQgraph(ch, x,y, XN   , YN   , MaxPdB, MinNdB);



    (*env)->ReleaseIntArrayElements(env, x_, x, 0);
    (*env)->ReleaseIntArrayElements(env, y_, y, 0);
    return;
}


JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeOutPEQgraph(JNIEnv *env, jobject instance, jint ch,
                                                       jintArray x_, jintArray y_, jint XN, jint YN,
                                                       jint MaxPdB, jint MinNdB) {
    jint *x = (*env)->GetIntArrayElements(env, x_, NULL);
    jint *y = (*env)->GetIntArrayElements(env, y_, NULL);

    // TODO
    OutPEQgraph(ch,x,y,XN,YN,MaxPdB,MinNdB);

    (*env)->ReleaseIntArrayElements(env, x_, x, 0);
    (*env)->ReleaseIntArrayElements(env, y_, y, 0);
}

JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeUpdateOutPEQ(JNIEnv *env, jobject instance, jint ch,
                                                        jint sec, jint type, jfloatArray Param_) {
    jfloat *Param = (*env)->GetFloatArrayElements(env, Param_, NULL);

    // TODO
    UpdateOutPEQ(ch,sec,type,Param);

    (*env)->ReleaseFloatArrayElements(env, Param_, Param, 0);
}

JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeUpdateOutLpHp(JNIEnv *env, jobject instance, jint ch,
                                                         jint filterType, jint filterAlgorithm,
                                                         jint filter_order, jfloat fc) {

    // TODO
    UpdateOutLpHp(ch,filterType,filterAlgorithm,filter_order,fc);

}

JNIEXPORT void JNICALL
Java_com_ckq_dasoftware_NativeMethod_nativeGlobalGraph(JNIEnv *env, jobject instance, jint ch,
                                                       jintArray x_, jintArray y_, jint XN, jint YN,
                                                       jint MaxPdB, jint MinNdB, jfloat gain) {
    jint *x = (*env)->GetIntArrayElements(env, x_, NULL);
    jint *y = (*env)->GetIntArrayElements(env, y_, NULL);

    // TODO
    GlobalGraph(ch,x,y,XN,YN,MaxPdB,MinNdB,gain);

    (*env)->ReleaseIntArrayElements(env, x_, x, 0);
    (*env)->ReleaseIntArrayElements(env, y_, y, 0);
}

