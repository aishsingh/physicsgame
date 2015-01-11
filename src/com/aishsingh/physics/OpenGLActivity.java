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
        int i = e.getActionIndex();
        float x = e.getX(i);
        float y = e.getY(i);

        switch (e.getAction() & e.getActionMasked() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                OpenGLLib.touchDown(x, y, i);
                break;

            case MotionEvent.ACTION_POINTER_DOWN:
                OpenGLLib.touchPointerDown(x, y, i);
                break;

            case MotionEvent.ACTION_MOVE:
                for (int p=0; p<e.getPointerCount(); p++) {
                    OpenGLLib.touchMove(e.getX(p), e.getY(p), p);
                }
                break;

            case MotionEvent.ACTION_UP:
                OpenGLLib.touchUp(i);
                break;

            case MotionEvent.ACTION_POINTER_UP:
                OpenGLLib.touchPointerUp(i);
                break;
        }

        // Event was handled
        return true; 
    }
}
