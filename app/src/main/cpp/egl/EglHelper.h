//
// Created by Shiki on 2020/6/17.
//

#ifndef NATIVEOPENGLDEMO_EGLHELPER_H
#define NATIVEOPENGLDEMO_EGLHELPER_H

#include <EGL/egl.h>
#include "../Constants.h"
#include "../AndroidLog.h"

class EglHelper {
public:

    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLConfig mEglConfig;
    EGLContext mEglContext = NULL;


    EglHelper();

    virtual ~EglHelper();

    int initEgl(EGLNativeWindowType win);

    int swapBuffers();

    void destoryEgl();
};


#endif //NATIVEOPENGLDEMO_EGLHELPER_H
