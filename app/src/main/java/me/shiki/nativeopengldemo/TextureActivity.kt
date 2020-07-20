package me.shiki.nativeopengldemo

import android.os.Bundle
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import kotlinx.android.synthetic.main.activity_textture.*
import kotlinx.coroutines.launch

class TextureActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_textture)
        gltv.textureRender.onRenderCreateListener = {
            lifecycleScope.launch {
                if (ll.childCount > 0) {
                    ll.removeAllViews()
                }

                for (i in 0 until 3) {
                    val mutiSurfaceView = MutiSurfaceView(this@TextureActivity)
                    mutiSurfaceView.setTextureId(it, i)
                    mutiSurfaceView.setSurfaceAndEglContext(eglContext = gltv.getEglContext())

                    val param = LinearLayout.LayoutParams(200, 300)
                    ll.addView(mutiSurfaceView, param)
                }
            }
        }
    }
}
