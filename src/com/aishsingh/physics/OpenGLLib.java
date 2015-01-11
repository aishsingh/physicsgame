package com.aishsingh.physics;

// Wrapper for native library
public class OpenGLLib {

     static {
         System.loadLibrary("opengl");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height); 
     public static native void draw();
     public static native void touchMove(float x, float y, int index);
     public static native void touchDown(float x, float y, int index);
     public static native void touchPointerDown(float x, float y, int index);
     public static native void touchPointerUp(int index);
     public static native void touchUp(int index);
     public static native void setAppName(String pkgname);

}
