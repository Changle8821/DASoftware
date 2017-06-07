package com.ckq.dasoftware;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

/**
 * Created by Administrator on 2017/6/2.
 */

public class DelayViewFragment extends Fragment implements View.OnClickListener{

    private Button polarity_in_a;
    private Button polarity_in_b;
    private Button polarity_in_c;
    private Button polarity_in_d;
    private Button polarity_out_1;
    private Button polarity_out_2;
    private Button polarity_out_3;
    private Button polarity_out_4;
    private Button polarity_out_5;
    private Button polarity_out_6;
    private Button polarity_out_7;
    private Button polarity_out_8;
    public static int flag = 0;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.delay,container,false);
        polarity_in_a= (Button) view.findViewById(R.id.polarity_btn_in_a);
        polarity_in_a.setOnClickListener(this);
        polarity_in_b= (Button) view.findViewById(R.id.polarity_btn_in_b);
        polarity_in_b.setOnClickListener(this);
        polarity_in_c= (Button) view.findViewById(R.id.polarity_btn_in_c);
        polarity_in_c.setOnClickListener(this);
        polarity_in_d= (Button) view.findViewById(R.id.polarity_btn_in_d);
        polarity_in_d.setOnClickListener(this);
        polarity_out_1= (Button) view.findViewById(R.id.polarity_btn_out_1);
        polarity_out_1.setOnClickListener(this);
        polarity_out_2= (Button) view.findViewById(R.id.polarity_btn_out_2);
        polarity_out_2.setOnClickListener(this);
        polarity_out_3= (Button) view.findViewById(R.id.polarity_btn_out_3);
        polarity_out_3.setOnClickListener(this);
        polarity_out_4= (Button) view.findViewById(R.id.polarity_btn_out_4);
        polarity_out_4.setOnClickListener(this);
        polarity_out_5= (Button) view.findViewById(R.id.polarity_btn_out_5);
        polarity_out_5.setOnClickListener(this);
        polarity_out_6= (Button) view.findViewById(R.id.polarity_btn_out_6);
        polarity_out_6.setOnClickListener(this);
        polarity_out_7= (Button) view.findViewById(R.id.polarity_btn_out_7);
        polarity_out_7.setOnClickListener(this);
        polarity_out_8= (Button) view.findViewById(R.id.polarity_btn_out_8);
        polarity_out_8.setOnClickListener(this);
        return view;
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.polarity_btn_in_a:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_in_a.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_in_a.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_in_b:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_in_b.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_in_b.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_in_c:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_in_c.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_in_c.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_in_d:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_in_d.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_in_d.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_1:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_1.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_1.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_2:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_2.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_2.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_3:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_3.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_3.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_4:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_4.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_4.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_5:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_5.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_5.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_6:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_6.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_6.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_7:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_7.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_7.setActivated(true);
                    flag = 0;
                }
                break;
            case R.id.polarity_btn_out_8:
                if (flag == 0) {
                    // 第一次单击触发的事件
                    polarity_out_8.setActivated(false);
                    flag = 1;
                } else {
                    // 第二次单击buttont改变触发的事件
                    polarity_out_8.setActivated(true);
                    flag = 0;
                }
                break;
            default:
                break;
        }

    }
}
