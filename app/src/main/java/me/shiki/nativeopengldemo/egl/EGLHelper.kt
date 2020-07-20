package me.shiki.livepusher.egl

import android.opengl.EGL14
import android.view.Surface
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLContext
import javax.microedition.khronos.egl.EGLDisplay
import javax.microedition.khronos.egl.EGLSurface

class EGLHelper {
    private var egl: EGL10? = null
    private var eglDisplay: EGLDisplay? = null
    var eglContext: EGLContext? = null
        private set
    private var eglSurface: EGLSurface? = null

    fun initEgl(surface: Surface?, context: EGLContext?) {
        //得到Egl实例
        egl = EGLContext.getEGL() as EGL10
        //得到默认的显示设备（就是窗口）
        eglDisplay = egl?.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY)

        //初始化默认显示设备
        val version = IntArray(2)
        if (egl?.eglInitialize(eglDisplay, version) != true) {
            throw RuntimeException("获取显示设备失败")
        }

        //设置显示设备的属性
        val attrbutes = intArrayOf(
            EGL10.EGL_RED_SIZE, 8,
            EGL10.EGL_GREEN_SIZE, 8,
            EGL10.EGL_BLUE_SIZE, 8,
            EGL10.EGL_ALPHA_SIZE, 8,
            EGL10.EGL_DEPTH_SIZE, 8,
            EGL10.EGL_STENCIL_SIZE, 8,
            EGL10.EGL_RENDERABLE_TYPE, 4,
            EGL10.EGL_NONE
        )

        val numConfig = IntArray(1)
        if (egl?.eglChooseConfig(eglDisplay, attrbutes, null, 1, numConfig) != true) {
            throw IllegalAccessException("配置失败")
        }

        val numConfigs = numConfig[0]
        if (numConfigs <= 0) {
            throw IllegalAccessException("沒有匹配的配置")
        }

        //从系统中获取对应属性的配置
        val configs = arrayOfNulls<EGLConfig>(numConfigs)
        if (egl?.eglChooseConfig(eglDisplay, attrbutes, configs, numConfigs, numConfig) != true) {
            throw IllegalAccessException("重新配置失败")
        }

        //创建EglContext
        val attribList = intArrayOf(
            EGL14.EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL10.EGL_NONE
        )
        eglContext = egl?.eglCreateContext(eglDisplay, configs[0], context ?: EGL10.EGL_NO_CONTEXT, attribList)

        //创建渲染的Surface
        eglSurface = egl?.eglCreateWindowSurface(eglDisplay, configs[0], surface, null)

        //绑定EglContext和Surface到显示设备中
        if (egl?.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) != true) {
            throw RuntimeException("绑定显示设备失败")
        }
    }

    fun swapBuffers(): Boolean {
        return egl?.eglSwapBuffers(eglDisplay, eglSurface) ?: false
    }

    fun destroyEgl() {
        egl?.let {
            it.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT)
            it.eglDestroySurface(eglDisplay, eglSurface)
            eglSurface = null

            it.eglDestroyContext(eglDisplay, eglContext)
            it.eglTerminate(eglDisplay)
            eglDisplay = null
            egl = null
        }
    }
}