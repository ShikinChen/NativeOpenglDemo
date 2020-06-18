//
// Created by Shiki on 2020/6/18.
//

#ifndef NATIVEOPENGLDEMO_EGLTHREAD_H
#define NATIVEOPENGLDEMO_EGLTHREAD_H

#include <pthread.h>
#include <android/native_window.h>
#include <EGL/eglplatform.h>
#include "EglHelper.h"
#include <unistd.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>

enum RenderType {
    AUTO,
    HANDLE
};

class EglThread {
public:
    pthread_t eglThread = -1;
    ANativeWindow *nativeWindow = NULL;

    void onSurfaceCreate(EGLNativeWindowType window);

    void onSurfaceChange(int width, int height);

    bool isCreate = false;
    bool isChange = false;
    bool isExit = false;
    bool isStart = false;

    int surfaceWidth = 0;
    int surfaceHeight = 0;

    RenderType renderType = AUTO;

    pthread_mutex_t pthreadMutex;
    pthread_cond_t pthreadCond;

    typedef void (*OnCreate)(void *context);

    OnCreate onCreate;

    void *onCreateContext;


    typedef void (*OnChange)(int width, int height, void *context);

    OnChange onChange;

    void *onChangeContext;


    typedef void (*OnDraw)(void *context);

    OnDraw onDraw;

    void *onDrawContext;


    EglThread();

    virtual ~EglThread();

    void callBackOnCeate(OnCreate onCreate, void *context);

    void callBackOnChange(OnChange onChange, void *context);

    void callBackOnDraw(OnDraw onDraw, void *context);

    void setRenderType(RenderType renderType);

    void notifyRender();
};


#endif //NATIVEOPENGLDEMO_EGLTHREAD_H
