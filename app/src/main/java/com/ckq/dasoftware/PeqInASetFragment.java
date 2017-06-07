package com.ckq.dasoftware;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

/**
 * Created by Administrator on 2017/4/9.
 */

public class PeqInASetFragment extends Fragment implements View.OnClickListener{

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
    private static final int low_pass = 1;
    private static final int high_pass = 2;
    private static final int low_shelf = 3;
    private static final int high_shelf = 4;
    private static final int all_pass = 5;
    public static int type;        // PEQ的类型：0：PEQ   1:Lowpass  2: Highpass 3: Lowshelf  4: Highshelf  5:allpass
    public static final int peak = 0;  // 滤波器类型：PEQ

    public NativeMethod jni = new NativeMethod();

    private Button sec_1;
    private Button sec_2;
    private Button sec_3;
    private Button sec_4;
    private Button sec_5;
    private Button sec_6;
    private Button sec_7;

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

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.peq_in_a_set_fragment,container,false);
        sec_1 = (Button) view.findViewById(R.id.peq_in_a_set_sec_1_click);
        sec_1.setOnClickListener(this);
        sec_2 = (Button) view.findViewById(R.id.peq_in_a_set_sec_2_click);
        sec_2.setOnClickListener(this);
        sec_3 = (Button) view.findViewById(R.id.peq_in_a_set_sec_3_click);
        sec_3.setOnClickListener(this);
        sec_4 = (Button) view.findViewById(R.id.peq_in_a_set_sec_4_click);
        sec_4.setOnClickListener(this);
        sec_5 = (Button) view.findViewById(R.id.peq_in_a_set_sec_5_click);
        sec_5.setOnClickListener(this);
        sec_6 = (Button) view.findViewById(R.id.peq_in_a_set_sec_6_click);
        sec_6.setOnClickListener(this);
        sec_7 = (Button) view.findViewById(R.id.peq_in_a_set_sec_7_click);
        sec_7.setOnClickListener(this);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        //        In A数据初始输入
//        sec = 0;                             // 设PEQ编号为1
//        type = peak;                         // 滤波器类型：PEQ
//        parameter[0]=40;                   // 频率值设为40Hz
//        parameter[1]= (float) 3;                      // Q值设为3
//        parameter[2]= 20;                      // 增益值设为0dB
//        jni.nativeUpdateInPEQ(ch,sec,peak,parameter);
//        jni.nativeInPEQgraph(ch,x,y,IN_PEQ_XN,IN_PEQ_YN,MaxInPEQdB,MinInPEQdB);
    }

    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.peq_in_a_set_sec_1_click:
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
                break;
        }
    }
}
