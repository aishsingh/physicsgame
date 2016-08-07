/* This file allows the Java and Native sources to interact together
 * It gets loaded as a library from java code (OpenGLLib.java)
 * It is used to Setup OpenGL ES2 then passes control to the renderer 
 * file through the mainloop
 */

#include <string>
#include <jni.h>
#include "game.h"
#include "log.h"
#include "unittests/tests.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_draw(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchPointerDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchPointerUp(JNIEnv * env, jobject obj, jint index);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchUp(JNIEnv * env, jobject obj, jint index);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_doubleTap(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_setAppName(JNIEnv * env, jobject obj, jstring pkgname);
};

Game *game = NULL;
char package_name;

/* NOTE: This is also run when screen sleeps then awakes */
JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj, jint width, jint height) {
    if (game == NULL) {
        runTests();

        // Create game instance if none have been created
        game = new Game(std::string(&package_name), width, height);
    }

    // resetup OpenGL going out of context (eg. Phone sleep)
    game->setupGLContext(width, height);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_draw(JNIEnv * env, jobject obj) {
    if (game)
        game->draw();
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index) {
    if (game)
        game->input.touchMove(x, y, index);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index) {
    if (game)
        game->input.touchDown(x, y, index);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchPointerDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint index) {
    if (game)
        game->input.touchPointerDown(x, y, index);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchPointerUp(JNIEnv * env, jobject obj, jint index) {
    if (game)
        game->input.touchPointerUp(index);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchUp(JNIEnv * env, jobject obj, jint index) {
    if (game)
        game->input.touchUp(index);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_doubleTap(JNIEnv * env, jobject obj, jfloat x, jfloat y) {
    if (game)
        game->input.doubleTap(x, y);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_setAppName( JNIEnv * env, jobject obj, jstring pkgname ) {
    // Get app name (fitmpGamelesystem location)
    const char *buffer = env->GetStringUTFChars(pkgname, 0);

    // copy to packageName
    strcpy( &package_name, buffer );

    //Free
    env->ReleaseStringUTFChars( pkgname, buffer );
}
