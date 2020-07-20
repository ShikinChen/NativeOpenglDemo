package me.shiki.livepusher.egl

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import me.shiki.commlib.constant.Consts
import me.shiki.nativeopengldemo.RenderMode
import java.lang.ref.WeakReference
import java.util.concurrent.locks.ReentrantLock
import javax.microedition.khronos.egl.EGLContext
import kotlin.concurrent.withLock

abstract class EGLSurfaceView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : SurfaceView(context, attrs, defStyleAttr), SurfaceHolder.Callback {
    private var surface: Surface? = null
    private var eglContext: EGLContext? = null

    private var eglThread: EGLThread? = null
    var render: EGLRender? = null

    var renderMode = RenderMode.CONTINUOUSLY

    init {
        holder.addCallback(this)
    }

    @JvmOverloads
    fun setSurfaceAndEglContext(
        surface: Surface? = null,
        eglContext: EGLContext?
    ) {
        this.surface = surface
        this.eglContext = eglContext
    }

    fun getEglContext(): EGLContext? {
        return eglThread?.getEglContext()
    }

    fun requestRender() {
        eglThread?.requestRender()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        holder?.let {
            if (surface == null) {
                surface = it.surface
            }
            eglThread = EGLThread(WeakReference(this))
            eglThread?.isCreate = true
            eglThread?.start()
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        eglThread?.width = width
        eglThread?.height = height
        eglThread?.isChange = true
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        eglThread?.onDestroy()
        eglThread = null
        surface = null
        eglContext = null
    }

    class EGLThread(private var eglSurfaceViewWeakReference: WeakReference<EGLSurfaceView>?) : Thread() {
        private var eglHelper: EGLHelper? = null
        private val lock by lazy {
            ReentrantLock()
        }
        private val condition by lazy {
            lock.newCondition()
        }
        private var isExit = false
        var isCreate = false
        var isChange = false
        private var isStart = false

        var width = 0
        var height = 0

        override fun run() {
            super.run()
            isExit = false
            isStart = false

            eglHelper = EGLHelper()
            eglHelper?.initEgl(
                eglSurfaceViewWeakReference?.get()?.surface,
                eglSurfaceViewWeakReference?.get()?.eglContext
            )

            while (!isExit) {
                if (isStart) {
                    with(eglSurfaceViewWeakReference?.get()?.renderMode) {
                        if (this == RenderMode.WHEN_DIRTY) {
                            lock.withLock {
                                condition.await()
                            }
                        } else {
                            sleep(Consts.FPS_TIME)
                        }
                    }
                }
                onCreate()
                onChange(width, height)
                onDraw()
                isStart = true
            }
            release()
        }

        private fun onCreate() {
            eglSurfaceViewWeakReference?.get()?.render?.let {
                if (isCreate) {
                    isCreate = false
                    it.onSurfaceCreated()
                }
            }
        }

        private fun onChange(width: Int, height: Int) {
            eglSurfaceViewWeakReference?.get()?.render?.let {
                if (isChange) {
                    isChange = false
                    it.onSurfaceChanged(width, height)
                }
            }
        }

        private fun onDraw() {
            with(eglSurfaceViewWeakReference?.get()?.render) {
                if (this != null && eglHelper != null) {
                    onDrawFrame()
                    if (!isStart) {
                        onDrawFrame()
                    }
                    eglHelper?.swapBuffers()
                }
            }
        }

        fun requestRender() {
            lock.withLock {
                condition.signal()
            }
        }

        fun onDestroy() {
            isExit = true
            requestRender()
        }

        private fun release() {
            eglHelper?.let {
                it.destroyEgl()
                eglHelper = null
                eglSurfaceViewWeakReference = null
            }
        }

        fun getEglContext(): EGLContext? {
            return eglHelper?.eglContext
        }
    }
}