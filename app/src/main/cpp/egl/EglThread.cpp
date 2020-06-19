//
// Created by Shiki on 2020/6/18.
//


#include "EglThread.h"

EglThread::EglThread() {
    pthread_mutex_init(&pthreadMutex, NULL);
    pthread_cond_init(&pthreadCond, NULL);
}

EglThread::~EglThread() {
    pthread_mutex_destroy(&pthreadMutex);
    pthread_cond_destroy(&pthreadCond);
}

void *eglThreadImpl(void *data) {
    EglThread *eglThread = static_cast<EglThread *>(data);
    EglHelper *eglHelper = new EglHelper();
    eglHelper->initEgl(eglThread->nativeWindow);
    eglThread->isExit = false;
    while (!eglThread->isExit) {
        if (eglThread->isCreate) {
            LOGD("eglThread 创建")
            eglThread->isCreate = false;
            if (eglThread->onCreate != NULL) {
                eglThread->onCreate(eglThread->onCreateContext);
            }
        }
        if (eglThread->isChange) {
            LOGD("eglThread Change")
            eglThread->isChange = false;
            if (eglThread->onChange != NULL) {
                eglThread->onChange(eglThread->surfaceWidth, eglThread->surfaceHeight, eglThread->onChangeContext);
            }
            eglThread->isStart = true;

        }

        if (eglThread->isStart) {
//            LOGD("draw")
            if (eglThread->onDraw != NULL) {
                eglThread->onDraw(eglThread->onDrawContext);
            }
            eglHelper->swapBuffers();
        }

        if (eglThread->renderType == AUTO) {
            usleep(1000000 / 60);
        } else if (eglThread->renderType == HANDLE) {
            pthread_mutex_lock(&eglThread->pthreadMutex);
            pthread_cond_wait(&eglThread->pthreadCond, &eglThread->pthreadMutex);
            pthread_mutex_unlock(&eglThread->pthreadMutex);
        }
    }
    return 0;
}

void EglThread::onSurfaceCreate(EGLNativeWindowType window) {
    if (eglThread == -1) {
        isCreate = true;
        nativeWindow = window;

        pthread_create(&eglThread, NULL, eglThreadImpl, this);
    }
}

void EglThread::onSurfaceChange(int width, int height) {
    isChange = true;
    surfaceHeight = height;
    surfaceWidth = width;
    notifyRender();
}

void EglThread::callBackOnCeate(EglThread::OnCreate onCreate, void *context) {
    this->onCreate = onCreate;
    this->onCreateContext = context;
}

void EglThread::callBackOnChange(EglThread::OnChange onChange, void *context) {
    this->onChange = onChange;
    this->onChangeContext = context;
}

void EglThread::callBackOnDraw(EglThread::OnDraw onDraw, void *context) {
    this->onDraw = onDraw;
    this->onDrawContext = context;
}

void EglThread::setRenderType(RenderType renderType) {
    this->renderType = renderType;
}

void EglThread::notifyRender() {
    pthread_mutex_lock(&pthreadMutex);
    pthread_cond_signal(&pthreadCond);
    pthread_mutex_unlock(&pthreadMutex);
}
