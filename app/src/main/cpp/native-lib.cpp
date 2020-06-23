#include <jni.h>
#include <string>
#include "AndroidLog.h"
#include "egl//EglHelper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>
#include "egl/EglThread.h"
#include "util/ShaderUtil.h"
#include "matrix/MatrixUtil.h"
#include "opengl/OpenglControl.h"

OpenglControl *openglControl = NULL;


extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {
    if (openglControl == NULL) {
        openglControl = new OpenglControl();
    }
    openglControl->onCreateSurface(env, surface);

}
extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceChange(JNIEnv *env, jobject thiz, jint width, jint height) {
    if (openglControl != NULL) {
        openglControl->onChangeSurface(width, height);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_imgData(JNIEnv *env, jobject thiz, jint w, jint h, jint length,
                                                    jbyteArray data_) {

    jbyte *data = env->GetByteArrayElements(data_, NULL);

    if (openglControl != NULL) {
        openglControl->setPixel(data, w, h, length);
    }

    env->ReleaseByteArrayElements(data_, data, 0);


}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceDestroy(JNIEnv *env, jobject thiz) {
    if (openglControl != NULL) {
        openglControl->onDestroySurface();
        delete openglControl;
        openglControl = NULL;
    }
}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceChangeFilter(JNIEnv *env, jobject thiz) {
    if (openglControl != NULL) {
        openglControl->onChangeFilterSurface();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_setYuvData(JNIEnv *env, jobject thiz, jbyteArray y_, jbyteArray u_,
                                                       jbyteArray v_, jint width, jint height) {
    jbyte *y = env->GetByteArrayElements(y_, NULL);
    jbyte *u = env->GetByteArrayElements(u_, NULL);
    jbyte *v = env->GetByteArrayElements(v_, NULL);
    if (openglControl != NULL) {
        openglControl->setYuvData(y, u, v, width, height);
    }

    env->ReleaseByteArrayElements(y_, y, 0);
    env->ReleaseByteArrayElements(u_, u, 0);
    env->ReleaseByteArrayElements(v_, v, 0);
}