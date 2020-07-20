package me.shiki.nativeopengldemo

import android.content.Context
import android.util.AttributeSet
import me.shiki.livepusher.egl.EGLSurfaceView

/**
 * MutiSurfaceView
 *
 * @author shiki
 * @date 2020/7/17
 *
 */
class MutiSurfaceView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) :
    EGLSurfaceView(context, attrs, defStyleAttr) {
    private val mutiRender by lazy {
        MutiRender()
    }

    init {
        render = mutiRender
    }

    fun setTextureId(textureId: Int, index: Int) {
        mutiRender.textureId = textureId
        mutiRender.index = index
    }
}