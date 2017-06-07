package com.ckq.dasoftware;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

/**
 * Created by Administrator on 2017/5/21.
 */

public class MainViewFragment extends Fragment implements View.OnClickListener{


    private Button buttonA;
    private Button buttonB;
    private Button buttonC;
    private Button buttonD;
    private Button button1;
    private Button button2;
    private Button button3;
    private Button button4;
    private Button button5;
    private Button button6;
    private Button button7;
    private Button button8;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.peq,container,false);
        buttonA = (Button) view.findViewById(R.id.peq_btn_in_a);
        buttonA.setOnClickListener(this);
        buttonB = (Button) view.findViewById(R.id.peq_btn_in_b);
        buttonB.setOnClickListener(this);
        buttonC = (Button) view.findViewById(R.id.peq_btn_in_c);
        buttonC.setOnClickListener(this);
        buttonD = (Button) view.findViewById(R.id.peq_btn_in_d);
        buttonD.setOnClickListener(this);
        button1 = (Button) view.findViewById(R.id.peq_btn_out_1);
        button1.setOnClickListener(this);
        button2 = (Button) view.findViewById(R.id.peq_btn_out_2);
        button2.setOnClickListener(this);
        button3 = (Button) view.findViewById(R.id.peq_btn_out_3);
        button3.setOnClickListener(this);
        button4 = (Button) view.findViewById(R.id.peq_btn_out_4);
        button4.setOnClickListener(this);
        button5 = (Button) view.findViewById(R.id.peq_btn_out_5);
        button5.setOnClickListener(this);
        button6 = (Button) view.findViewById(R.id.peq_btn_out_6);
        button6.setOnClickListener(this);
        button7 = (Button) view.findViewById(R.id.peq_btn_out_7);
        button7.setOnClickListener(this);
        button8 = (Button) view.findViewById(R.id.peq_btn_out_8);
        button8.setOnClickListener(this);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

    }



    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.peq_btn_in_a:
                setAllUnclicked();
                buttonA.setEnabled(false);
                break;
            case R.id.peq_btn_in_b:
                setAllUnclicked();
                buttonB.setEnabled(false);
                break;
            case R.id.peq_btn_in_c:
                setAllUnclicked();
                buttonC.setEnabled(false);
                break;
            case R.id.peq_btn_in_d:
                setAllUnclicked();
                buttonD.setEnabled(false);
                break;
            case R.id.peq_btn_out_1:
                setAllUnclicked();
                button1.setEnabled(false);
                break;
            case R.id.peq_btn_out_2:
                setAllUnclicked();
                button2.setEnabled(false);
                break;
            case R.id.peq_btn_out_3:
                setAllUnclicked();
                button3.setEnabled(false);
                break;
            case R.id.peq_btn_out_4:
                setAllUnclicked();
                button4.setEnabled(false);
                break;
            case R.id.peq_btn_out_5:
                setAllUnclicked();
                button5.setEnabled(false);
                break;
            case R.id.peq_btn_out_6:
                setAllUnclicked();
                button6.setEnabled(false);
                break;
            case R.id.peq_btn_out_7:
                setAllUnclicked();
                button7.setEnabled(false);
                break;
            case R.id.peq_btn_out_8:
                setAllUnclicked();
                button8.setEnabled(false);
                break;
            default:
                break;
        }
    }

    private void setAllUnclicked() {
        buttonA.setEnabled(true);
        buttonB.setEnabled(true);
        buttonC.setEnabled(true);
        buttonD.setEnabled(true);
        button1.setEnabled(true);
        button2.setEnabled(true);
        button3.setEnabled(true);
        button4.setEnabled(true);
        button5.setEnabled(true);
        button6.setEnabled(true);
        button7.setEnabled(true);
        button8.setEnabled(true);  //把所有的声道按钮都置为没有按过状态
    }

}
