/* This file allows the Java and Native sources to interact together
 * It gets loaded as a library from java code (OpenGLLib.java)
 * It is used to Setup OpenGL ES2 then passes control to the renderer 
 * file through the mainloop
 */

#include <jni.h>
#include "game.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y);
};

// static Game *game = NULL;
static Game game;

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj,  jint width, jint height) {
    // if (game) {
    //     delete game;
    //     game = NULL;
    // }
    // static Game tmpGame;
    // tmpGame.renderer.setupGraphics(width, height);
    // game = &tmpGame;
    game.setup(width, height);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_step(JNIEnv * env, jobject obj) {
    // if (game) {
    //     game->run();
    // }
    game.run();
}


JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y) {
    game.handleInput(x, y);
}
