//
// Created by Shiki on 2020/6/19.
//

#ifndef NATIVEOPENGLDEMO_SHADERUTIL_H
#define NATIVEOPENGLDEMO_SHADERUTIL_H

#include "../Constants.h"
#include <GLES2/gl2.h>

/**
 * 创建shader
 * @param shaderType
 * @param code
 * @return
 */
static int loadShaders(int shaderType, const char *code) {
    //创建shader（着色器：顶点或片元
    int shader = glCreateShader(shaderType);
    //加载shader源码并编译shader
    glShaderSource(shader, 1, &code, 0);
    glCompileShader(shader);
    return shader;
}

/**
 * 创建一个渲染程序
 * @param vertex 顶点源码
 * @param fragment 片元源码
 * @return
 */
static int createProgram(const char *vertex, const char *fragment, GLuint *vShader, GLuint *fShader) {
    GLuint vertexShader = loadShaders(GL_VERTEX_SHADER, vertex);
    GLuint fragmentShader = loadShaders(GL_FRAGMENT_SHADER, fragment);
    int program = glCreateProgram();
    //将着色器程序添加到渲染程序中
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    //链接源程序
    glLinkProgram(program);

    *vShader = vertexShader;
    *fShader = fragmentShader;
    return program;
}

#endif //NATIVEOPENGLDEMO_SHADERUTIL_H
