package me.shiki.nativeopengldemo

import android.view.Surface

/**
 * @author shiki
 * @since 2020/6/18 9:48 AM
 */
class NativeOpengl {
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    external fun surfaceCreate(surface: Surface?)

    external fun surfaceChange(width: Int, height: Int)

    external fun imgData(width: Int, height: Int, length: Int, data: ByteArray)

    external fun setYuvData(y: ByteArray, u: ByteArray, v: ByteArray, width: Int, height: Int)

    external fun surfaceDestroy()

    external fun surfaceChangeFilter()
}