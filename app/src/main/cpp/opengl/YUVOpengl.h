//
// Created by Shiki on 2020/6/22.
//

#ifndef NATIVEOPENGLDEMO_YUVOPENGL_H
#define NATIVEOPENGLDEMO_YUVOPENGL_H

#include "../egl/EglThread.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "BaseOpengl.h"

class YUVOpengl : public BaseOpengl {
public:

    GLint vPosition;
    GLint fPosition;

    GLint samplerY;
    GLint samplerU;
    GLint samplerV;
    GLint uMatrix;

    GLuint samplers[3];
    float matrix[16];

    void *y = NULL;
    void *u = NULL;
    void *v = NULL;

    int yuvWidth = 0;
    int yuvHeight = 0;

    YUVOpengl();

    virtual ~YUVOpengl();

    void onCreate();

    void onChange(int width, int height);

    void onDraw();

    void setOrthoM();

    void setYuvData(void *y, void *u, void *v, int width, int height);

    void destroy() override;

private:
    void freeYUV();
};


#endif //NATIVEOPENGLDEMO_YUVOPENGL_H
