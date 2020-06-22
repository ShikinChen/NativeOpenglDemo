//
// Created by Shiki on 2020/6/21.
//

#include "OpenglControl.h"
#include "ImgOpengl.h"

OpenglControl::OpenglControl() {

}

OpenglControl::~OpenglControl() {

}


void callbackSurfaceCreate(void *context) {
    OpenglControl *openglControl = static_cast<OpenglControl *>(context);
    if (openglControl != NULL) {
        if (openglControl->baseOpengl != NULL) {
            openglControl->baseOpengl->onCreate();
        }
    }

}

void callbackSurfaceChange(int w, int h, void *context) {
    OpenglControl *openglControl = static_cast<OpenglControl *>(context);
    if (openglControl != NULL) {
        if (openglControl->baseOpengl != NULL) {
            openglControl->baseOpengl->onChange(w, h);
        }
    }
}


void callbackSurfaceDraw(void *context) {
    OpenglControl *openglControl = static_cast<OpenglControl *>(context);
    if (openglControl != NULL) {
        if (openglControl->baseOpengl != NULL) {
            openglControl->baseOpengl->onDraw();
        }
    }
}

void OpenglControl::onCreateSurface(JNIEnv *env, jobject surface) {
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    eglThread = new EglThread();
    eglThread->setRenderType(HANDLE);
    eglThread->callBackOnCeate(callbackSurfaceCreate, this);
    eglThread->callBackOnChange(callbackSurfaceChange, this);
    eglThread->callBackOnDraw(callbackSurfaceDraw, this);

    baseOpengl = new ImgOpengl();

    eglThread->onSurfaceCreate(nativeWindow);
}


void OpenglControl::onChangeSurface(int width, int height) {
    if (eglThread != NULL) {
        if (baseOpengl != NULL) {
            baseOpengl->surfaceWidth = width;
            baseOpengl->surfaceHeight = height;
        }
        eglThread->onSurfaceChange(width, height);
    }
}


void OpenglControl::onDestorySurface() {
    if (eglThread != NULL) {
        eglThread->destory();
    }
    if (baseOpengl != NULL) {
        baseOpengl->destory();
        delete baseOpengl;
        baseOpengl = NULL;
    }
    if (nativeWindow != NULL) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = NULL;
    }
    free(pixel);
    pixel = NULL;
}

void OpenglControl::setPixel(void *data, int width, int height, int length) {
    imgHeight = height;
    imgWidth = width;

    pixel = malloc(length);
    memcpy(pixel, data, length);

    if (baseOpengl != NULL) {
        baseOpengl->setPixel(pixel, width, height);
    }
    if (eglThread != NULL) {
        eglThread->notifyRender();
    }
}

