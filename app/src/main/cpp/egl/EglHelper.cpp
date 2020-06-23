//
// Created by Shiki on 2020/6/17.
//

#include "EglHelper.h"

EglHelper::EglHelper() {
    mEglDisplay = EGL_NO_DISPLAY;
    mEglSurface = EGL_NO_SURFACE;
    mEglContext = EGL_NO_CONTEXT;
}

EglHelper::~EglHelper() {

}

int EglHelper::initEgl(EGLNativeWindowType win) {
    //得到默认的显示设备（就是窗口）
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEglDisplay == NULL) {
        LOGE("eglDisplay 获取失败")
        return CODE_ERROR;
    }
    //初始化默认显示设备
    EGLint *version = new EGLint[2];
    if (!eglInitialize(mEglDisplay, &version[0], &version[1])) {
        LOGE("egl 初始化失败")
        return CODE_ERROR;
    }
    //设置显示设备的属性
    const EGLint attribs[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_STENCIL_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
    };
    EGLint numConfig;
    if (!eglChooseConfig(mEglDisplay, attribs, NULL, 1, &numConfig)) {
        LOGE("egl 配置失败")
        return CODE_ERROR;
    }
    //从系统中获取对应属性的配置
    if (!eglChooseConfig(mEglDisplay, attribs, &mEglConfig, numConfig, &numConfig)) {
        LOGE("egl 配置失败")
        return CODE_ERROR;
    }
    //创建EglContext
    int attribList[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE,
    };
    mEglContext = eglCreateContext(mEglDisplay, mEglConfig, EGL_NO_CONTEXT, attribList);

    if (mEglContext == EGL_NO_CONTEXT) {
        LOGE("egl 创建上下文失败")
        return CODE_ERROR;
    }

    //创建渲染的Surface
    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, win, NULL);
    if (mEglSurface == EGL_NO_SURFACE) {
        LOGE("egl 创建Surface失败")
        return CODE_ERROR;
    }
    //绑定EglContext和Surface到显示设备中
    if (!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        LOGE("egl 绑定EglContext和Surface失败")
        return CODE_ERROR;
    }
    LOGE("egl 初始化成功")
    return CODE_SUCCESS;
}

int EglHelper::swapBuffers() {
    if (mEglDisplay == EGL_NO_DISPLAY || mEglSurface == EGL_NO_SURFACE) {
        LOGE("egl 还没初始化")
        return CODE_ERROR;
    }
    if (!eglSwapBuffers(mEglDisplay, mEglSurface)) {
        LOGE("egl 刷新数据失败")
        return CODE_ERROR;
    }
    return CODE_SUCCESS;
}

void EglHelper::destroyEgl() {
    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mEglDisplay, mEglSurface);
        mEglSurface = EGL_NO_SURFACE;
    }
    if (mEglDisplay != EGL_NO_DISPLAY && mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
        mEglContext = EGL_NO_CONTEXT;
    }
    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglTerminate(mEglDisplay);
        mEglDisplay = EGL_NO_DISPLAY;
    }
}
