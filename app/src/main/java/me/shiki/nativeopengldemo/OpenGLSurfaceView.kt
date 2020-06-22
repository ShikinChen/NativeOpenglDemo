package me.shiki.nativeopengldemo

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

/**
 * @author shiki
 * @since 2020/6/18 9:52 AM
 */
class OpenGLSurfaceView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : SurfaceView(context, attrs, defStyleAttr), SurfaceHolder.Callback {

    var nativeOpengl: NativeOpengl? = null

    var surfaceCreatedListener: (() -> Unit)? = null

    init {
        holder?.addCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeOpengl?.surfaceChange(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        nativeOpengl?.surfaceDestory()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        nativeOpengl?.surfaceCreate(holder?.surface)
        surfaceCreatedListener?.invoke()
    }
}