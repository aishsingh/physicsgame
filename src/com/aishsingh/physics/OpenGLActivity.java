package com.aishsingh.physics;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.MotionEvent;

import java.io.File;


public class OpenGLActivity extends Activity {

    OpenGLView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new OpenGLView(getApplication());
	setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.

        float x = e.getX();
        float y = e.getY();

        boolean touchDown = true;

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                OpenGLLib.touchMove(x,y);
                break;

            case MotionEvent.ACTION_MOVE:
                OpenGLLib.touchMove(x,y);
                break;

            case MotionEvent.ACTION_UP:
                touchDown = false;
                break;
        }

        return touchDown;
    }
}
