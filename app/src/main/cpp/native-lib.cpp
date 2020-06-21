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

int program;
GLint vPosition;
float vertexs[] = {
        1, -1,
        1, 1,
        -1, -1,
        -1, 1,
};

float matrix[16];

//图片

const char *img_vertex = "attribute vec4 v_Position;\n"
                         "attribute vec2 f_Position;\n"
                         "varying vec2 ft_Position;\n"
                         "uniform mat4 u_Matrix;\n"
                         "void main() {\n"
                         "    ft_Position = f_Position;\n"
                         "    gl_Position = v_Position*u_Matrix;\n"
                         "}";


const char *img_fragment = "precision mediump float;\n"
                           "varying vec2 ft_Position;\n"
                           "uniform sampler2D sTexture;\n"
                           "void main() {\n"
                           "    gl_FragColor=texture2D(sTexture, ft_Position);\n"
                           "}";


float fragments[] = {
        1, 1,
        1, 0,
        0, 1,
        0, 0,
};

GLint v_Position;
GLint f_Position;
GLint sTexture;
GLuint textureId;
GLuint u_Matrix;

int width;
int height;
void *pixels = NULL;

void callbackSurfaceCreate(void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    program = createProgram(img_vertex, img_fragment);
    LOGD("opengl program %d", program)
    //矩形
    vPosition = glGetAttribLocation(program, "a_position");

    //图片
    v_Position = glGetAttribLocation(program, "v_Position");
    f_Position = glGetAttribLocation(program, "f_Position");
    sTexture = glGetUniformLocation(program, "sTexture");
    u_Matrix = glGetUniformLocation(program, "u_Matrix");

    if (sTexture >= 0) {
        initMatrix(matrix);
//        rotateMatrixByZ(matrix, 60);
//        scaleMatrix(matrix, 3.5);
//        transMatrix(matrix, 1, -1);


        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (pixels != NULL) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }
    }

    //解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
}

void callbackSurfaceChange(int w, int h, void *context) {
    EglThread *eglThread = static_cast<EglThread *>(context);
    glViewport(0, 0, w, h);

    float sreen_r = w * 1.0 / h;
    float img_r = width * 1.0 / height;

    if (sreen_r > img_r) {//宽度缩放
        float r = w / (h * 1.0 / height * width);
        orthoM(matrix, r, 1);
    } else {//高度缩放
        float r = h / (w * 1.0 / width * height);
        orthoM(matrix, 1, r);
    }
}


void callbackSurfaceDraw(void *context) {
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

    //图片
    if (v_Position >= 0) {
        glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, matrix);


        //绑定第五个纹理
        glActiveTexture(GL_TEXTURE5);
        glUniform1i(sTexture, 5);


        glBindTexture(GL_TEXTURE_2D, textureId);

        glEnableVertexAttribArray(v_Position);
        glVertexAttribPointer(v_Position, 2, GL_FLOAT, false, 8, vertexs);

        glEnableVertexAttribArray(f_Position);
        glVertexAttribPointer(f_Position, 2, GL_FLOAT, false, 8, fragments);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / (sizeof(float) * 2));
        glBindTexture(GL_TEXTURE_2D, 0);
    }


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
}extern "C"
JNIEXPORT void JNICALL
Java_me_shiki_nativeopengldemo_NativeOpengl_imgData(JNIEnv *env, jobject thiz, jint w, jint h, jint length,
                                                    jbyteArray data_) {

    jbyte *data = env->GetByteArrayElements(data_, NULL);

    width = w;
    height = h;

    pixels = malloc(length);
    memcpy(pixels, data, length);

    env->ReleaseByteArrayElements(data_, data, 0);
}