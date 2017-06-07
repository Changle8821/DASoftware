package com.ckq.dasoftware;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.NumberPicker;
import android.widget.Spinner;
import android.widget.TextView;

import com.ckq.dasoftware.utils.ImageUtils;
import com.ckq.dasoftware.utils.ScreenUtils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import static com.ckq.dasoftware.utils.ScreenUtils.dip2px;

/**
 * Created by Administrator on 2017/4/9.
 */

public class PeqInALine extends View{


    public   int ch = 0 ; // 输入声道编号, 取值范围0~3（In A -> 0， In B -> 1  In C -> 2， In D -> 3）,设声道为In A
    public   int sec ;            // 第几个PEQ，取值范围0~6
    public static final int IN_PEQ_XN = 865;         // IN PEQ x轴的象数点
    public static final int IN_PEQ_YN = 310;         // IN PEQ y轴的象数点
    int[] x = new int[IN_PEQ_XN];             // 用于画图的x轴的值
    int[] y = new int[IN_PEQ_XN];             // 用于画图的y轴的值
    public static final int  MaxInPEQdB = 20;          // PEQ y轴的最大dB值
    public static final int MinInPEQdB = -40;         // PEQ y轴的最小dB值

    public static final int IN_PEQ_PARAM_NUM = 3;  // IN PEQ 的参数个数
    float[] parameter = new float[IN_PEQ_PARAM_NUM];// PEQ对应的3个参数，排列顺序为：fc,Q,gain

    public static int type;        // PEQ的类型：0：PEQ   1:Lowpass  2: Highpass 3: Lowshelf  4: Highshelf  5:allpass
    public static final int peak = 0;  // 滤波器类型：PEQ
    private static final int low_pass = 1;
    private static final int high_pass = 2;
    private static final int low_shelf = 3;
    private static final int high_shelf = 4;
    private static final int all_pass = 5;

    public NativeMethod jni = new NativeMethod();

    public Path mPath;
    private Point[] mPoints;// 曲线的总点数
    private Context mContext;
    private Paint mLinePaint;
    private Paint mTextPaint;
    private Paint mGridPaint;



    private static final int UPDATE_1 = 1;
    private static final int UPDATE_2 = 2;
    private static final int UPDATE_3 = 3;
    private static final int UPDATE_4 = 4;
    private static final int UPDATE_5 = 5;
    private static final int UPDATE_6 = 6;
    private static final int UPDATE_7 = 7;

    private String text = "";
    private Spinner filterTypeSelect_1;
    private EditText peq_channel_item1_freq_digit;
    private EditText peq_channel_item1_Q_digit;
    private EditText peq_channel_item1_gain_digit;

    private Spinner filterTypeSelect_2;
    private EditText peq_channel_item2_freq_digit;
    private EditText peq_channel_item2_Q_digit;
    private EditText peq_channel_item2_gain_digit;

    private Spinner filterTypeSelect_3;
    private EditText peq_channel_item3_freq_digit;
    private EditText peq_channel_item3_Q_digit;
    private EditText peq_channel_item3_gain_digit;

    private Spinner filterTypeSelect_4;
    private EditText peq_channel_item4_freq_digit;
    private EditText peq_channel_item4_Q_digit;
    private EditText peq_channel_item4_gain_digit;

    private Spinner filterTypeSelect_5;
    private EditText peq_channel_item5_freq_digit;
    private EditText peq_channel_item5_Q_digit;
    private EditText peq_channel_item5_gain_digit;

    private Spinner filterTypeSelect_6;
    private EditText peq_channel_item6_freq_digit;
    private EditText peq_channel_item6_Q_digit;
    private EditText peq_channel_item6_gain_digit;

    private Spinner filterTypeSelect_7;
    private EditText peq_channel_item7_freq_digit;
    private EditText peq_channel_item7_Q_digit;
    private EditText peq_channel_item7_gain_digit;




    public PeqInALine(Context context) {
        this(context, null);
    }

    public PeqInALine(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        this.mContext = context;
        setClickable(true);
        init();
    }



    public void init() {

        mLinePaint = new Paint(); // 曲线画笔
        mLinePaint.setColor(Color.RED);
        mLinePaint.setStrokeWidth(5);
        mLinePaint.setStyle(Paint.Style.STROKE);
        mLinePaint.setAntiAlias(true);
        mPath = new Path(); //曲线路径

        mTextPaint  = new Paint(); // 画笔
        mTextPaint.setColor(Color.RED);
        mTextPaint.setStrokeWidth(2);
        mTextPaint.setTextSize(24);
        mTextPaint.setStyle(Paint.Style.STROKE);
        mTextPaint.setAntiAlias(true);

        mGridPaint = new Paint(); // 画笔
        mGridPaint.setColor(Color.RED);
        mGridPaint.setStrokeWidth(1);
        mGridPaint.setStyle(Paint.Style.STROKE);
        mGridPaint.setAntiAlias(true);


//        filterTypeSelect_1= (Spinner) findViewById(R.id.filterTypeSelect_1);
////        filterTypeSelect_1.setOnClickListener(this);
//        peq_channel_item1_freq_digit= (EditText) findViewById(R.id.peq_channel_item1_freq_digit);
//        parameter[0]= Integer.parseInt(peq_channel_item1_freq_digit.getText().toString());
//        peq_channel_item1_freq_digit.setOnEditorActionListener(new TextView.OnEditorActionListener() {
//            @Override
//            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
//                //关闭输入法
//                InputMethodManager inputMethodManager = (InputMethodManager)
//                        QueryAddressActivity.this.getSystemService
//                                (Context.INPUT_METHOD_SERVICE);
//                inputMethodManager.hideSoftInputFromWindow(etQueryPhone.getWindowToken(), 0);
//                return true;
//            }
//        });
//        peq_channel_item1_freq_digit.setOnClickListener(this);
//        peq_channel_item1_Q_digit=(EditText) findViewById(R.id.peq_channel_item1_Q_digit);
////        peq_channel_item1_Q_digit.setOnClickListener(this);
//        peq_channel_item1_gain_digit=(EditText) findViewById(R.id.peq_channel_item1_gain_digit);
//        peq_channel_item1_gain_digit.setOnClickListener(this);
//
//        filterTypeSelect_2= (Spinner) findViewById(R.id.filterTypeSelect_2);
//        filterTypeSelect_2.setOnClickListener(this);
//        peq_channel_item2_freq_digit= (EditText) findViewById(R.id.peq_channel_item2_freq_digit);
//        peq_channel_item2_freq_digit.setOnClickListener(this);
//        peq_channel_item2_Q_digit=(EditText) findViewById(R.id.peq_channel_item2_Q_digit);
//        peq_channel_item2_Q_digit.setOnClickListener(this);
//        peq_channel_item2_gain_digit=(EditText) findViewById(R.id.peq_channel_item2_gain_digit);
//        peq_channel_item2_gain_digit.setOnClickListener(this);
//
//        filterTypeSelect_3= (Spinner) findViewById(R.id.filterTypeSelect_3);
//        filterTypeSelect_3.setOnClickListener(this);
//        peq_channel_item3_freq_digit= (EditText) findViewById(R.id.peq_channel_item3_freq_digit);
//        peq_channel_item3_freq_digit.setOnClickListener(this);
//        peq_channel_item3_Q_digit=(EditText) findViewById(R.id.peq_channel_item3_Q_digit);
//        peq_channel_item3_Q_digit.setOnClickListener(this);
//        peq_channel_item3_gain_digit=(EditText) findViewById(R.id.peq_channel_item3_gain_digit);
//        peq_channel_item3_gain_digit.setOnClickListener(this);
//
//        filterTypeSelect_4= (Spinner) findViewById(R.id.filterTypeSelect_4);
//        filterTypeSelect_4.setOnClickListener(this);
//        peq_channel_item4_freq_digit= (EditText) findViewById(R.id.peq_channel_item4_freq_digit);
//        peq_channel_item4_freq_digit.setOnClickListener(this);
//        peq_channel_item4_Q_digit=(EditText) findViewById(R.id.peq_channel_item4_Q_digit);
//        peq_channel_item4_Q_digit.setOnClickListener(this);
//        peq_channel_item4_gain_digit=(EditText) findViewById(R.id.peq_channel_item4_gain_digit);
//        peq_channel_item4_gain_digit.setOnClickListener(this);
//
//        filterTypeSelect_5= (Spinner) findViewById(R.id.filterTypeSelect_5);
//        filterTypeSelect_5.setOnClickListener(this);
//        peq_channel_item5_freq_digit= (EditText) findViewById(R.id.peq_channel_item5_freq_digit);
//        peq_channel_item5_freq_digit.setOnClickListener(this)ById(R.id.peq_channel_item5_gain_digit);
//        peq_channel_item5_gain_digit.setOnClickListener(;
//        peq_channel_item5_Q_digit=(EditText) findViewById(R.id.peq_channel_item5_Q_digit);
//        peq_channel_item5_Q_digit.setOnClickListener(this);
//        peq_channel_item5_gain_digit=(EditText) findViewthis);
//
//        filterTypeSelect_6= (Spinner) findViewById(R.id.filterTypeSelect_6);
//        filterTypeSelect_6.setOnClickListener(this);
//        peq_channel_item6_freq_digit= (EditText) findViewById(R.id.peq_channel_item6_freq_digit);
//        peq_channel_item6_freq_digit.setOnClickListener(this);
//        peq_channel_item6_Q_digit=(EditText) findViewById(R.id.peq_channel_item6_Q_digit);
//        peq_channel_item6_Q_digit.setOnClickListener(this);
//        peq_channel_item6_gain_digit=(EditText) findViewById(R.id.peq_channel_item6_gain_digit);
//        peq_channel_item6_gain_digit.setOnClickListener(this);
//
//        filterTypeSelect_7= (Spinner) findViewById(R.id.filterTypeSelect_7);
//        filterTypeSelect_7.setOnClickListener(this);
//        peq_channel_item7_freq_digit= (EditText) findViewById(R.id.peq_channel_item7_freq_digit);
//        peq_channel_item7_freq_digit.setOnClickListener(this);
//        peq_channel_item7_Q_digit=(EditText) findViewById(R.id.peq_channel_item7_Q_digit);
//        peq_channel_item7_Q_digit.setOnClickListener(this);
//        peq_channel_item7_gain_digit=(EditText) findViewById(R.id.peq_channel_item7_gain_digit);
//        peq_channel_item7_gain_digit.setOnClickListener(this);

    }





//    private ArrayAdapter<String> adapter;                                                                //存放数据
//
//    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
//        String cardNumber = this.getResources().getStringArray(R.array.filterType)[arg2];
//    }

//    private Handler handler = new Handler(){
//        public void handleMessage(Message msg){
//            switch (msg.what){
//                case UPDATE_1:
//                    sec = 0 ;
//                    type = 1;
//                    parameter[0]= Integer.parseInt(peq_channel_item1_freq_digit.getText().toString());
//                    parameter[1]= Float.parseFloat(peq_channel_item1_Q_digit.getText().toString());
//                    parameter[2]= Float.parseFloat(peq_channel_item1_gain_digit.getText().toString());
//                    jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                    jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//
//                    break;
//                default:
//                    break;
//            }
//        }
//    };


//    private void drawLinesPath(Canvas canvas){
//
//
//        canvas.drawPath(mPath,mLinePaint);
//
////        sec = 1;                             // 设PEQ编号为2
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=100;                   // 频率值设为100Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        sec = 2;                             // 设PEQ编号为3
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=200;                   // 频率值设为200Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        sec = 3;                             // 设PEQ编号为4
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=500;                   // 频率值设为500Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        sec = 4;                             // 设PEQ编号为5
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=1000;                   // 频率值设为1000Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        sec = 5;                             // 设PEQ编号为6
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=2000;                   // 频率值设为2000Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        sec = 6;                             // 设PEQ编号为7
////        type = peak;                         // 滤波器类型：PEQ
////        parameter[0]=5000;                   // 频率值设为5000Hz
////        parameter[1]= (float) 3;                      // Q值设为3
////        parameter[2]= 0;                      // 增益值设为0dB
////        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
////        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
////        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
////        for (int i = 1; i < x.length ; i++ ){
////            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
////        }
////        canvas.drawPath(mPath,mLinePaint);
////
////        invalidate();
//
//    }




//        switch (v.getId()){
//            case R.id.peq_in_a_set_sec_1_click:
//                break;
//                new Thread(new Runnable() {
//                    @Override
//                    public void run() {
////                        filterTypeSelect_1.setOnItemClickListener(new AdapterView.OnItemClickListener() {
////                            @Override
////                            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
////                                text = (String) filterTypeSelect_1.getSelectedItem(); //获取spinner选中的值
////                            }
////                        });
////                        if (text.equals("PEQ")){
////                            type = 0;
////                        }else if (text.equals("L_Pass")){
////                            type =1;
////                        }else if (text.equals("H_Pass")){
////                            type=2;
////                        }else if (text.equals("L_Shelf")){
////                            type=3;
////                        }else if (text.equals("H_Shelf")){
////                            type=4;
////                        }else if (text.equals("AllPass")){
////                            type=5;
////                        }
//                        Message message = new Message();
//                        message.what = UPDATE_1;
//                        handler.sendMessage(message);
//                    }
//                }).start();
//                break;
//            default:
//                break;
//        }

//
//        switch (v.getId()) {
//            case R.id.peq_in_a_set_sec_1_click:
//                filterTypeSelect_1.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_1.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")) {
//                    type = 0;
//                } else if (text.equals("L_Pass")) {
//                    type = 1;
//                } else if (text.equals("H_Pass")) {
//                    type = 2;
//                } else if (text.equals("L_Shelf")) {
//                    type = 3;
//                } else if (text.equals("H_Shelf")) {
//                    type = 4;
//                } else if (text.equals("AllPass")) {
//                    type = 5;
//                }
//                type = peak;
//                parameter[0] = Integer.parseInt(peq_channel_item1_freq_digit.getText().toString());
//                parameter[1] = Float.parseFloat(peq_channel_item1_Q_digit.getText().toString());
//                parameter[2] = Float.parseFloat(peq_channel_item1_gain_digit.getText().toString());
//                sec = 0;
//                jni.nativeUpdateInPEQ(ch, sec, peak, parameter);
//                jni.nativeInPEQgraph(ch, x, y, IN_PEQ_XN, IN_PEQ_YN, MaxInPEQdB, MinInPEQdB);
////                super.invalidate();
//
//                break;
//
//            case R.id.filterTypeSelect_2:
//            case R.id.peq_channel_item2_freq_digit:
//            case R.id.peq_channel_item2_Q_digit:
//            case R.id.peq_channel_item2_gain_digit:
//                filterTypeSelect_2.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_2.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item2_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item2_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item2_gain_digit.getText().toString());
//                sec = 1;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            case R.id.filterTypeSelect_3:
//            case R.id.peq_channel_item3_freq_digit:
//            case R.id.peq_channel_item3_Q_digit:
//            case R.id.peq_channel_item3_gain_digit:
//                filterTypeSelect_3.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_3.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item3_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item3_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item3_gain_digit.getText().toString());
//                sec = 2;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            case R.id.filterTypeSelect_4:
//            case R.id.peq_channel_item4_freq_digit:
//            case R.id.peq_channel_item4_Q_digit:
//            case R.id.peq_channel_item4_gain_digit:
//                filterTypeSelect_4.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_4.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item4_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item4_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item4_gain_digit.getText().toString());
//                sec = 3;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            case R.id.filterTypeSelect_5:
//            case R.id.peq_channel_item5_freq_digit:
//            case R.id.peq_channel_item5_Q_digit:
//            case R.id.peq_channel_item5_gain_digit:
//                filterTypeSelect_5.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_5.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item5_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item5_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item5_gain_digit.getText().toString());
//                sec = 4;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            case R.id.filterTypeSelect_6:
//            case R.id.peq_channel_item6_freq_digit:
//            case R.id.peq_channel_item6_Q_digit:
//            case R.id.peq_channel_item6_gain_digit:
//                filterTypeSelect_6.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_6.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item6_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item6_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item6_gain_digit.getText().toString());
//                sec = 5;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            case R.id.filterTypeSelect_7:
//            case R.id.peq_channel_item7_freq_digit:
//            case R.id.peq_channel_item7_Q_digit:
//            case R.id.peq_channel_item7_gain_digit:
//                filterTypeSelect_7.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//                    @Override
//                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//                        text = (String) filterTypeSelect_7.getSelectedItem(); //获取spinner选中的值
//                    }
//                });
//                if (text.equals("PEQ")){
//                    type = 0;
//                }else if (text.equals("L_Pass")){
//                    type =1;
//                }else if (text.equals("H_Pass")){
//                    type=2;
//                }else if (text.equals("L_Shelf")){
//                    type=3;
//                }else if (text.equals("H_Shelf")){
//                    type=4;
//                }else if (text.equals("AllPass")){
//                    type=5;
//                }
//                parameter[0]= Integer.parseInt(peq_channel_item7_freq_digit.getText().toString());
//                parameter[1]= Float.parseFloat(peq_channel_item7_Q_digit.getText().toString());
//                parameter[2]= Float.parseFloat(peq_channel_item7_gain_digit.getText().toString());
//                sec = 6;
//                jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//                jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//                super.invalidate();
//                break;
//
//            default:
//                break;
//        }



    private void drawAllXLine(Canvas canvas){

    }



    @Override
    protected void onDraw(Canvas canvas) {


        super.onDraw(canvas);

        canvas.scale(1, 0.8f);



        int h = getHeight();
        int w = getWidth();


        mPath.moveTo(0,0);
        mPath.lineTo(0,IN_PEQ_YN);
        canvas.drawPath(mPath,mLinePaint);

        mPath.moveTo(0,0);
        mPath.lineTo(IN_PEQ_XN,0);
        canvas.drawPath(mPath,mLinePaint);

        mPath.moveTo(IN_PEQ_XN,0);
        mPath.lineTo(IN_PEQ_XN,IN_PEQ_YN);
        canvas.drawPath(mPath,mLinePaint);


        mPath.moveTo(0,IN_PEQ_YN);
        mPath.lineTo(IN_PEQ_XN,IN_PEQ_YN);
        canvas.drawPath(mPath,mLinePaint);


        //        In A数据初始输入
        sec = 0;                             // 设PEQ编号为1
        type = peak ;                        // 滤波器类型：0:PEQ  1:Lowpass  2: Highpass 3: Lowshelf  4: Highshelf  5:allpass ;
                                            // peak=0;  low_pass = 1; high_pass = 2; low_shelf = 3; high_shelf = 4; all_pass = 5;
        parameter[0]=40;                   // 频率值设为40Hz
        parameter[1]= (float) 3;                      // Q值设为3
        parameter[2]= 10;                      // 增益值设为0dB
        jni.nativeUpdateInPEQ(ch,sec,peak ,parameter);
        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
        for (int i = 1; i < x.length ; i++ ){
            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
        }
        canvas.drawPath(mPath,mLinePaint);

//        sec = 1;                             // 设PEQ编号为1
//        type =  high_pass  ;                        // 滤波器类型：0:PEQ  1:Lowpass  2: Highpass 3: Lowshelf  4: Highshelf  5:allpass ;
//        // peak=0;  low_pass = 1; high_pass = 2; low_shelf = 3; high_shelf = 4; all_pass = 5;
//        parameter[0]=1000;                   // 频率值设为40Hz
//        parameter[1]= (float) 1;                      // Q值设为3
//        parameter[2]= -10;                      // 增益值设为0dB
//        jni.nativeUpdateInPEQ(ch,sec,  high_pass ,parameter);
//        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
//        for (int i = 1; i < x.length ; i++ ){
//            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
//        }
//        canvas.drawPath(mPath,mLinePaint);

//        //        In A数据初始输入
//        sec = 1;                             // 设PEQ编号为1
//        type = high_shelf ;                         // 滤波器类型：PEQ
//        parameter[0]=1000;                   // 频率值设为40Hz
//        parameter[1]= (float) 3;                      // Q值设为3
//        parameter[2]= -10;                      // 增益值设为0dB
//        jni.nativeUpdateInPEQ(ch,sec,high_shelf ,parameter);
//        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
//        for (int i = 1; i < x.length ; i++ ){
//            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
//        }
//        canvas.drawPath(mPath,mLinePaint);

//
//        //        In A数据初始输入
//        sec = 3;                             // 设PEQ编号为2
//        type = all_pass;                         // 滤波器类型：low_shelf
//        parameter[0]=400;                   // 频率值设为40Hz
//        parameter[1]= (float) 3;                      // Q值设为3
//        parameter[2]= 10;                      // 增益值设为0dB
//        jni.nativeUpdateInPEQ(ch,sec,all_pass,parameter);
//        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
//        mPath.moveTo(x[0],IN_PEQ_YN-y[0]);
//        for (int i = 1; i < x.length ; i++ ){
//            mPath.lineTo(x[i],IN_PEQ_YN-y[i]);
//        }
//        canvas.drawPath(mPath,mLinePaint);



    }
}
