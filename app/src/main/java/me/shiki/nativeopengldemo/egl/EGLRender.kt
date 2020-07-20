package me.shiki.livepusher.egl

interface EGLRender {
    fun onSurfaceCreated()
    fun onSurfaceChanged(width: Int, height: Int)
    fun onDrawFrame()
}