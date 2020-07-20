package me.shiki.nativeopengldemo

inline class RenderModeInline(private val value: Int) {
    override fun toString(): String {
        return value.toString()
    }
}

object RenderMode {
    val WHEN_DIRTY = RenderModeInline(0)
    val CONTINUOUSLY = RenderModeInline(1)
}