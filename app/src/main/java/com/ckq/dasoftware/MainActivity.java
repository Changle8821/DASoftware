package com.ckq.dasoftware;

import android.support.v4.app.Fragment;
import android.graphics.Point;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;


import java.util.ArrayList;


/**
 * Created by Administrator on 2017/4/9.
 */

public class MainActivity extends AppCompatActivity implements View.OnClickListener {


    private Button peq_btn;
    private Button delay_btn;
    private Button limit_btn;
    private Button login_btn;

    private MainViewFragment mPeqView;
    private DelayViewFragment mDelayView;
    private LimitViewFragment mLimitView;
    private LoginViewFragment mLoginView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null){
            actionBar.hide();
        }
        init();
        replaceFragment(new MainViewFragment());
    }

    private void replaceFragment(Fragment fragment){
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction transaction = fragmentManager.beginTransaction();
        transaction.replace(R.id.main_view,fragment);
        transaction.commit();
    }

    private void init(){

        peq_btn = (Button) findViewById(R.id.peq_btn);
        peq_btn.setOnClickListener(this);
        delay_btn= (Button) findViewById(R.id.delay_btn);
        delay_btn.setOnClickListener(this);
        limit_btn= (Button) findViewById(R.id.limit_btn);
        limit_btn.setOnClickListener(this);
        login_btn= (Button) findViewById(R.id.login_btn);
        login_btn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.peq_btn:
                setAllUnclickedView();
                peq_btn.setEnabled(false);
                if (mPeqView == null)
                {
                    replaceFragment(new MainViewFragment());
                }
                break;
            case R.id.delay_btn:
                setAllUnclickedView();
                delay_btn.setEnabled(false);
                if (mDelayView == null)
                {
                    replaceFragment(new DelayViewFragment());
                }
                break;
            case R.id.limit_btn:
                setAllUnclickedView();
                limit_btn.setEnabled(false);
                if (mLimitView == null)
                {
                    replaceFragment(new LimitViewFragment());
                }
                break;
            case R.id.login_btn:
                setAllUnclickedView();
                login_btn.setEnabled(false);
                if (mLoginView == null)
                {
                    replaceFragment(new LoginViewFragment());
                }
                break;

        }

    }

    private void setAllUnclickedView(){
        peq_btn.setEnabled(true);
        delay_btn.setEnabled(true);
        limit_btn.setEnabled(true);
        login_btn.setEnabled(true);
    }
}
