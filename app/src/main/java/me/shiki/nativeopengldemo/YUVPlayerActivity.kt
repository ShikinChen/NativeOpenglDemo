package me.shiki.nativeopengldemo

import android.os.Bundle
import android.os.Environment
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.yanzhenjie.permission.AndPermission
import com.yanzhenjie.permission.runtime.Permission
import kotlinx.android.synthetic.main.activity_yuv_player.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.File
import java.lang.Exception

class YUVPlayerActivity : AppCompatActivity() {
    private var isExit = true

    private val nativeOpengl by lazy {
        NativeOpengl()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_yuv_player)
        osv.nativeOpengl = nativeOpengl

        osv.surfaceCreatedListener = {
        }

        btn_play.setOnClickListener {
            if (isExit) {
                AndPermission.with(this)
                    .runtime()
                    .permission(Permission.Group.STORAGE)
                    .onGranted {
                        lifecycleScope.launch(Dispatchers.IO) {
                            isExit = false
                            val width = 352
                            val height = 288
                            val y = ByteArray(width * height)
                            val u = ByteArray(width * height / 4)
                            val v = ByteArray(width * height / 4)
                            try {
                                val inputStream =
                                    File(Environment.getExternalStorageDirectory().absolutePath + "/test.yuv").inputStream()
                                while (!isExit) {
                                    val ySize = inputStream.read(y)
                                    val uSize = inputStream.read(u)
                                    val vSize = inputStream.read(v)
                                    if (ySize > 0 && uSize > 0 && vSize > 0) {
                                        nativeOpengl.setYuvData(y, u, v, width, height)
                                        Thread.sleep(60)
                                    } else {
                                        isExit = true
                                    }
                                }
                            } catch (e: Exception) {
                                Log.e(this::class.java.name, e.message)
                            }
                        }
                    }
                    .onDenied {}
                    .start()
            }
        }

        btn_stop.setOnClickListener {
            isExit = true
        }
    }
}
