//
// Created by Shiki on 2020/6/21.
//

#include "OpenglControl.h"
#include "ImgOpengl.h"
#include "RectangleOpengl.h"
#include "ImgOpengl.h"
#include "GrayImgOpengl.h"
#include "YUVOpengl.h"

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

void callbackSurfaceChangeFilter(int w, int h, void *context) {
    OpenglControl *openglControl = static_cast<OpenglControl *>(context);
    if (openglControl != NULL) {
        if (openglControl->baseOpengl != NULL) {
            openglControl->baseOpengl->destroy();
            delete openglControl->baseOpengl;
            openglControl->baseOpengl = NULL;
        }
        openglControl->baseOpengl = new GrayImgOpengl();
        openglControl->baseOpengl->onCreate();
        openglControl->baseOpengl->onChange(w, h);
        openglControl->baseOpengl->setPixel(openglControl->pixel, openglControl->imgWidth, openglControl->imgHeight);

    }
}

void callbackSurfaceDestroy(void *context) {
    OpenglControl *openglControl = static_cast<OpenglControl *>(context);
    if (openglControl != NULL) {
        openglControl->baseOpengl->destroy();
    }
}

void OpenglControl::onCreateSurface(JNIEnv *env, jobject surface) {
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    eglThread = new EglThread();
    eglThread->setRenderType(HANDLE);
    eglThread->callBackOnCeate(callbackSurfaceCreate, this);
    eglThread->callBackOnChange(callbackSurfaceChange, this);
    eglThread->callBackOnDraw(callbackSurfaceDraw, this);
    eglThread->callBackOnChangeFilter(callbackSurfaceChangeFilter, this);
    eglThread->callBackOnDestroy(callbackSurfaceDestroy, this);

    baseOpengl = new YUVOpengl();

    eglThread->onSurfaceCreate(nativeWindow);
}


void OpenglControl::onChangeSurface(int width, int height) {
    if (eglThread != NULL) {
        eglThread->onSurfaceChange(width, height);
    }
}


void OpenglControl::onDestroySurface() {
    if (eglThread != NULL) {
        eglThread->destroy();
    }
    if (baseOpengl != NULL) {
        baseOpengl->destroy();
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
    if (pixel != NULL) {
        free(pixel);
        pixel = NULL;
    }
    pixel = malloc(length);
    memcpy(pixel, data, length);

    if (baseOpengl != NULL) {
        baseOpengl->setPixel(pixel, width, height);
    }
    if (eglThread != NULL) {
        eglThread->notifyRender();
    }
}

void OpenglControl::onChangeFilterSurface() {
    if (eglThread != NULL) {
        eglThread->onSurfaceChangeFilter();
    }
}

void OpenglControl::setYuvData(void *y, void *u, void *v, int width, int height) {
    if (baseOpengl != NULL) {
        baseOpengl->setYuvData(y, u, v, width, height);
    }
    if (eglThread != NULL) {
        eglThread->notifyRender();
    }
}

