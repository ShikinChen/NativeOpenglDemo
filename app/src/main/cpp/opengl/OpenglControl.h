//
// Created by Shiki on 2020/6/21.
//

#ifndef NATIVEOPENGLDEMO_OPENGLCONTROL_H
#define NATIVEOPENGLDEMO_OPENGLCONTROL_H

#include "../egl/EglThread.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "BaseOpengl.h"

class OpenglControl {
public:
    EglThread *eglThread = NULL;
    ANativeWindow *nativeWindow = NULL;

    BaseOpengl *baseOpengl = NULL;

    int imgWidth;

    int imgHeight;

    void *pixel=NULL;

    OpenglControl();

    virtual ~OpenglControl();

    void onCreateSurface(JNIEnv *env, jobject surface);

    void onChangeSurface(int width, int height);

    void onChangeFilterSurface();

    void onDestroySurface();

    void setPixel(void *data, int width, int height, int length);

    void setYuvData(void *y, void * u, void * v, int width,int height);
};


#endif //NATIVEOPENGLDEMO_OPENGLCONTROL_H
