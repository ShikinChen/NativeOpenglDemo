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
#include <unordered_map>
#include "CallJava.h"

using namespace std;
OpenglControl *openglControl = NULL;
CallJava *callJava = NULL;
JavaVM *javaVm = NULL;
static std::unordered_map<int, OpenglControl *> *g_instanceDic;

extern "C"
JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVm = vm;
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {

    if (callJava == NULL) {
        callJava = new CallJava(javaVm, env, &thiz);
    }
    int hashCode = callJava->onCallHashCode(&thiz);
    if (hashCode < 0) {
        return;
    }
    openglControl = NULL;
    if (g_instanceDic == NULL) {
        g_instanceDic = new unordered_map<int, OpenglControl *>;
    }
    auto itr = g_instanceDic->find(hashCode);
    if (itr != g_instanceDic->end()) {
        openglControl = itr->second;
    }
    if (openglControl == NULL) {
        openglControl = new OpenglControl();
        (*g_instanceDic)[hashCode] = openglControl;
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

        int hashCode = callJava->onCallHashCode(&thiz);
        if (hashCode >= 0) {
            auto itr = g_instanceDic->find(hashCode);
            if (itr != g_instanceDic->end()) {
                OpenglControl *openglControl = itr->second;
                openglControl->onDestroySurface();
                delete openglControl;
                g_instanceDic->erase(hashCode);
            }
        }


//        openglControl = NULL;
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