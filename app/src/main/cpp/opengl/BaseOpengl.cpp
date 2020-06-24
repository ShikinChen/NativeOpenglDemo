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
    vertexsSize = sizeof(v) / sizeof(float);
    fragmentsSize = sizeof(f) / sizeof(float);

    memcpy(vertexs, v, sizeof(v));
    memcpy(fragments, f, sizeof(f));
}

BaseOpengl::~BaseOpengl() {
    delete[]vertexs;
    delete[]fragments;
}

void BaseOpengl::onCreate() {
    //创建vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexsSize * 4 + fragmentsSize * 4, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexsSize * 4, vertexs);

    glBufferSubData(GL_ARRAY_BUFFER, vertexsSize * 4, fragmentsSize * 4, fragments);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //创建fbo
    glGenFramebuffers(1, &fbo);
}

void BaseOpengl::onChange(int width, int height) {
    surfaceHeight = height;
    surfaceWidth = width;
}

void BaseOpengl::onDraw() {

}

void BaseOpengl::destroy() {
    glDeleteBuffers(1, &vbo);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textureId);
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteProgram(program);
}

void BaseOpengl::setPixel(void *data, int width, int height) {

}

void BaseOpengl::setYuvData(void *y, void *u, void *v, int width, int height) {

}
