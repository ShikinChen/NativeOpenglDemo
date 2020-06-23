//
// Created by Shiki on 2020/6/21.
//

#ifndef NATIVEOPENGLDEMO_GRAYIMGOPENGL_H
#define NATIVEOPENGLDEMO_GRAYIMGOPENGL_H


#include "BaseOpengl.h"

class GrayImgOpengl : public BaseOpengl {
public:


    GLint v_Position;
    GLint f_Position;
    GLint sTexture;
    GLuint textureId;
    GLuint u_Matrix;

    int width;
    int height;
    void *pixel = NULL;

    float matrix[16];

    GrayImgOpengl();

    virtual ~GrayImgOpengl();

    void onCreate();

    void onChange(int width, int height);

    void onDraw();

    void setOrthoM();

    void setPixel(void *data, int width, int height);
};


#endif //NATIVEOPENGLDEMO_GRAYIMGOPENGL_H
