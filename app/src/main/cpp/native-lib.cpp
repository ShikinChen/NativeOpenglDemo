#include <jni.h>
#include <string>
#include "AndroidLog.h"
#include "egl//EglHelper.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>
#include "egl/EglThread.h"
#include "util/ShaderUtil.h"
#include "matrix/MatrixUtil.h"
#include "opengl/OpenglControl.h"

ANativeWindow *nativeWindow = NULL;
EglThread *eglThread = NULL;
//矩形
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


float vertexs[] = {
        1, -1,
        1, 1,
        -1, -1,
        -1, 1,
};

int program;
GLint vPosition;

OpenglControl *openglControl = NULL;

void callbackSurfaceCreate1(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
//    program = createProgram(vertex, fragment);
    LOGD("opengl program %d", program)
    //矩形
    vPosition = glGetAttribLocation(program, "a_position");


    //解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
}


void callbackSurfaceChange1(int w, int h, void *context) {
//    EglThread *eglThread = static_cast<EglThread *>(context);
//    glViewport(0, 0, w, h);
}


void callbackSurfaceDraw1(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    //矩形
    if (vPosition >= 0) {
        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 8, vertexs);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / (sizeof(float) * 2));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {

//    nativeWindow = ANativeWindow_fromSurface(env, surface);
//    eglThread = new EglThread();
////    eglThread->setRenderType(HANDLE);
//    eglThread->callBackOnCeate(callbackSurfaceCreate, eglThread);
//    eglThread->callBackOnChange(callbackSurfaceChange, eglThread);
//    eglThread->callBackOnDraw(callbackSurfaceDraw, eglThread);
//    eglThread->onSurfaceCreate(nativeWindow);
    if (openglControl == NULL) {
        openglControl = new OpenglControl();
    }
    openglControl->onCreateSurface(env, surface);

}
extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceChange(JNIEnv *env, jobject thiz, jint width, jint height) {
    if (openglControl != NULL) {
        openglControl->onChangeSurface(width, height);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_imgData(JNIEnv *env, jobject thiz, jint w, jint h, jint length,
                                                    jbyteArray data_) {

    jbyte *data = env->GetByteArrayElements(data_, NULL);

    if (openglControl != NULL) {
        openglControl->setPixel(data, w, h, length);
    }

    env->ReleaseByteArrayElements(data_, data, 0);


}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_surfaceDestory(JNIEnv *env, jobject thiz) {
    if (openglControl != NULL) {
        openglControl->onDestorySurface();
        delete openglControl;
        openglControl = NULL;
    }
}