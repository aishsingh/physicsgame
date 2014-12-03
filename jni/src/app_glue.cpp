/* This file allows the Java and Native sources to interact together
 * It gets loaded as a library from java code (OpenGLLib.java)
 * It is used to Setup OpenGL ES2 then passes control to the renderer 
 * file through the mainloop
 */

#include <string>
#include <jni.h>
#include "game.h"
#include "log.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y);

    JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_setAppName( JNIEnv * env, jobject obj, jstring pkgname );
};

static Game *game;
static char packageName;
bool gameStarted;

/* FIXME This is also run when screen sleeps then awakes */
JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_init(JNIEnv * env, jobject obj,  jint width, jint height) {
    // Clear unix term
    // LOGI("\033c");

    // Destroy any old instance
    if (!gameStarted) {
        // Create new game
        game = new Game();

        // Unzip apk using libzip
        game->loadAPK(&packageName);
        game->setup(width, height, packageName);

        gameStarted = true;
    }
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_step(JNIEnv * env, jobject obj) {
    game->run();
}


JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_touchMove(JNIEnv * env, jobject obj, jfloat x, jfloat y) {
    game->handleInput(x, y);
}

JNIEXPORT void JNICALL Java_com_aishsingh_physics_OpenGLLib_setAppName( JNIEnv * env, jobject obj, jstring pkgname ) {
    // Get app name (fitmpGamelesystem location)
    const char *buffer = env->GetStringUTFChars(pkgname, 0);

    // copy to packageName
    strcpy( &packageName, buffer );

    //Free
    env->ReleaseStringUTFChars( pkgname, buffer );
}

void setupTex() {

}

