#include <jni.h>
#include <string>
#include "AndroidLog.h"
#include "egl//EglHelper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>
#include "egl/EglThread.h"

ANativeWindow *nativeWindow = NULL;
EglThread *eglThread = NULL;

void callbackSurfaceCreate(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
}

void callbackSurfaceChange(int width, int height, void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    glViewport(0, 0, width, height);
}

float rColor = 0;

void callbackSurfaceDraw(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    rColor += 0.01;
    if (rColor > 1) {
        rColor = 0;
    }
    glClearColor(rColor, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {

    nativeWindow = ANativeWindow_fromSurface(env, surface);
    eglThread = new EglThread();
//    eglThread->setRenderType(HANDLE);
    eglThread->callBackOnCeate(callbackSurfaceCreate, eglThread);
    eglThread->callBackOnChange(callbackSurfaceChange, eglThread);
    eglThread->callBackOnDraw(callbackSurfaceDraw, eglThread);
    eglThread->onSurfaceCreate(nativeWindow);


}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceChange(JNIEnv *env, jobject thiz, jint width, jint height) {
    if (eglThread != NULL) {
        eglThread->onSurfaceChange(width, height);
    }
}