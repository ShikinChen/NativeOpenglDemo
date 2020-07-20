package me.shiki.livepusher.egl

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.opengl.GLES20
import android.opengl.GLUtils
import java.nio.ByteBuffer

/**
 * Shader工具类
 */
class ShaderUtil {
    companion object {
        /**
         * 生成Shader
         */
        @JvmStatic
        fun loadShader(shaderType: Int, source: String): Int {
            var shader = GLES20.glCreateShader(shaderType)
            if (shader != 0) {
                GLES20.glShaderSource(shader, source)
                GLES20.glCompileShader(shader)

                val compile = IntArray(1)
                GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compile, 0)
                if (compile[0] != GLES20.GL_TRUE) {
                    GLES20.glDeleteShader(shader)
                    shader = 0
                }
            }
            return shader
        }

        /**
         * 创建Program
         */
        @JvmStatic
        fun createProgram(vertexSource: String, fragmentSource: String): Int {
            var program = 0
            val vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource)
            val fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource)

            if (vertexShader != 0 && fragmentShader != 0) {
                program = GLES20.glCreateProgram()

                GLES20.glAttachShader(program, vertexShader)
                GLES20.glAttachShader(program, fragmentShader)

                GLES20.glLinkProgram(program)
            }

            return program
        }

        @JvmStatic
        @JvmOverloads
        fun createTextImage(
            text: String,
            textSize: Float,
            textColor: String = "#ff000000",
            bgColor: String = "#00000000",
            padding: Int = 0
        ): Bitmap {
            val paint = Paint()
            paint.color = Color.parseColor(textColor)
            paint.textSize = textSize
            paint.style = Paint.Style.FILL
            paint.isAntiAlias = true
            val width = paint.measureText(text, 0, text.length)
            val top = paint.fontMetrics.top
            val bottom = paint.fontMetrics.bottom
            val bm = Bitmap.createBitmap(
                (width + padding * 2).toInt(),
                (bottom - top + padding * 2).toInt(),
                Bitmap.Config.ARGB_8888
            )
            val canvas = Canvas(bm)
            canvas.drawColor(Color.parseColor(bgColor))
            canvas.drawText(text, padding.toFloat(), -top + padding, paint)
            return bm
        }

        @JvmStatic
        fun loadBitmapTexture(bitmap: Bitmap): Int {
            val textureIds = IntArray(1)
            GLES20.glGenTextures(1, textureIds, 0)
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureIds[0])
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
            val bitmapBuffer =
                ByteBuffer.allocate(bitmap.height * bitmap.width * 4)
            bitmap.copyPixelsToBuffer(bitmapBuffer)
            bitmapBuffer.flip()
            GLES20.glTexImage2D(
                GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, bitmap.width,
                bitmap.height, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, bitmapBuffer
            )
            return textureIds[0]
        }

        @JvmStatic
        fun loadTexrute(srcImg: Int, context: Context): Int {
            val textureIds = IntArray(1)
            GLES20.glGenTextures(1, textureIds, 0)
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureIds[0])

            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)

            var bitmap = BitmapFactory.decodeResource(context.resources, srcImg)
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0)
            bitmap.recycle()
            bitmap = null
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
            return textureIds[0]
        }
    }
}