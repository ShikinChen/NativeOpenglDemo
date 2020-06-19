package me.shiki.nativeopengldemo

import android.graphics.BitmapFactory
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {

    val nativeOpengl by lazy {
        NativeOpengl()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        osv.nativeOpengl = nativeOpengl

        val bitmap = BitmapFactory.decodeResource(resources, R.drawable.test)
        val buffer = ByteBuffer.allocate(bitmap.height * bitmap.width * 4);
        bitmap.copyPixelsToBuffer(buffer)
        buffer.flip()
        val pixels = buffer.array()
        nativeOpengl.imgData(bitmap.width, bitmap.height, pixels.size, pixels)
    }
}
