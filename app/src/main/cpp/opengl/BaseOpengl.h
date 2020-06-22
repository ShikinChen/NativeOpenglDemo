//
// Created by Shiki on 2020/6/21.
//

#ifndef NATIVEOPENGLDEMO_BASEOPENGL_H
#define NATIVEOPENGLDEMO_BASEOPENGL_H


#include <GLES2/gl2.h>
#include <string.h>
#include "../util/ShaderUtil.h"
#include "../matrix/MatrixUtil.h"
#include "../AndroidLog.h"

class BaseOpengl {
public:

    int surfaceWidth;
    int surfaceHeight;

    char *vertex;
    char *fragment;


    float *vertexs;
    float *fragments;

    GLuint program;


    BaseOpengl();

    virtual ~BaseOpengl();

    virtual void onCreate();

    virtual void onChange(int width, int height);

    virtual void onDraw();

    virtual void destory();

    virtual void setPixel(void *data, int width, int height);
};


#endif //NATIVEOPENGLDEMO_BASEOPENGL_H
