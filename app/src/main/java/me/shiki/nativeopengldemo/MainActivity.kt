package me.shiki.nativeopengldemo

import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {

    private val nativeOpengl by lazy {
        NativeOpengl()
    }

    private val imgs: IntArray = intArrayOf(R.drawable.test01, R.drawable.test02, R.drawable.test03)

    private var index = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        osv.nativeOpengl = nativeOpengl

        osv.surfaceCreatedListener = {
            readPixel(imgs[index])
        }
        btn_filter.setOnClickListener {
            nativeOpengl.surfaceChangeFilter()
        }
        btn_texture.setOnClickListener {
            index++
            if (index >= imgs.size) {
                index = 0
            }
            readPixel(imgs[index])
        }
        btn_yuv.setOnClickListener {
            startActivity(Intent(this, YUVPlayerActivity::class.java))
        }
        btn_go_texture.setOnClickListener {
            startActivity(Intent(this, TextureActivity::class.java))
        }
    }

    private fun readPixel(resId: Int) {
        var bitmap: Bitmap? = BitmapFactory.decodeResource(resources, resId)
        if (bitmap != null) {
            val buffer = ByteBuffer.allocate(bitmap.height * bitmap.width * 4)
            bitmap.copyPixelsToBuffer(buffer)
            buffer.flip()
            val pixels = buffer.array()
            nativeOpengl.imgData(bitmap.width, bitmap.height, pixels.size, pixels)
        }
    }
}
