//
// Created by Shiki on 2020/6/21.
//

#include "BaseOpengl.h"

BaseOpengl::BaseOpengl() {
    vertexs = new float[8];
    fragments = new float[8];

    float v[] = {
            1, -1,
            1, 1,
            -1, -1,
            -1, 1,
    };

    float f[] = {
            1, 1,
            1, 0,
            0, 1,
            0, 0,
    };
    memcpy(vertexs, v, sizeof(v));
    memcpy(fragments, f, sizeof(f));
}

BaseOpengl::~BaseOpengl() {
    delete []vertexs;
    delete []fragments;
}

void BaseOpengl::onCreate() {

}

void BaseOpengl::onChange(int width, int height) {

}

void BaseOpengl::onDraw() {

}

void BaseOpengl::destory() {

}

void BaseOpengl::setPixel(void *data, int width, int height) {

}
