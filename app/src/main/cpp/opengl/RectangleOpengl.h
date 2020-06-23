//
// Created by Shiki on 2020/6/22.
//

#ifndef NATIVEOPENGLDEMO_RECTANGLEOPENGL_H
#define NATIVEOPENGLDEMO_RECTANGLEOPENGL_H


#include "BaseOpengl.h"

class RectangleOpengl : public BaseOpengl {
public:


    RectangleOpengl();

    virtual ~RectangleOpengl();

    void onCreate() override;

    void onChange(int width, int height) override;

    void onDraw() override;

    GLint vPosition;
};


#endif //NATIVEOPENGLDEMO_RECTANGLEOPENGL_H
