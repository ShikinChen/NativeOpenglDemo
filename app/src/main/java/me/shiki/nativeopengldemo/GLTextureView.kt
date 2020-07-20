package me.shiki.nativeopengldemo

import android.content.Context
import android.util.AttributeSet
import me.shiki.livepusher.egl.EGLSurfaceView

/**
 * GLTextureView
 *
 * @author shiki
 * @date 2020/7/17
 *
 */
class GLTextureView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) :
    EGLSurfaceView(context, attrs, defStyleAttr) {
    val textureRender: TextureRender by lazy {
        TextureRender(context)
    }

    init {
        render = textureRender
    }
}