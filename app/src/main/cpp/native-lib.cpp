#include <jni.h>
#include <string>
#include "AndroidLog.h"
#include "egl//EglHelper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>
#include "egl/EglThread.h"
#include "util/ShaderUtil.h"

ANativeWindow *nativeWindow = NULL;
EglThread *eglThread = NULL;

const char *vertex = "attribute vec4 a_position;\n"
                     "\n"
                     "void main(){\n"
                     "    gl_Position = a_position;\n"
                     "}";
const char *fragment = "precision mediump float;\n"
                       "\n"
                       "void main(){\n"
                       "    gl_FragColor = vec4(1f,0f,0f,1f);\n"
                       "}";
int program;
GLint vPosition;
float vertexs[] = {
        -1, -1,
        1, -1,
        -1, 1,

        1, 1,
};

void callbackSurfaceCreate(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    program = createProgram(vertex, fragment);
    LOGD("opengl program %d", program)

    vPosition = glGetAttribLocation(program, "a_position");
}

void callbackSurfaceChange(int width, int height, void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    glViewport(0, 0, width, height);
}


void callbackSurfaceDraw(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 8, vertexs);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(float));
}

extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {

    nativeWindow = ANativeWindow_fromSurface(env, surface);
    eglThread = new EglThread();
//    eglThread->setRenderType(HANDLE);
    eglThread->callBackOnCeate(callbackSurfaceCreate, eglThread);
    eglThread->callBackOnChange(callbackSurfaceChange, eglThread);
    eglThread->callBackOnDraw(callbackSurfaceDraw, eglThread);
    eglThread->onSurfaceCreate(nativeWindow);


}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceChange(JNIEnv *env, jobject thiz, jint width, jint height) {
    if (eglThread != NULL) {
        eglThread->onSurfaceChange(width, height);
    }
}