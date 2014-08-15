#ifndef LOG_H
#define LOG_H

#include <android/log.h>
#include <GLES2/gl2.h>

#define  LOG_TAG    "JNIPhysicsGame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);

#endif /* LOG_H */
