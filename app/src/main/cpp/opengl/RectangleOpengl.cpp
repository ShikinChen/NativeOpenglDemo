//
// Created by Shiki on 2020/6/22.
//

#include "RectangleOpengl.h"

RectangleOpengl::RectangleOpengl() {
    vertex = "attribute vec4 a_position;\n"
             "\n"
             "void main(){\n"
             "    gl_Position = a_position;\n"
             "}";
    fragment = "precision mediump float;\n"
               "\n"
               "void main(){\n"
               "    gl_FragColor = vec4(1f,0f,0f,1f);\n"
               "}";

    float v[] = {
            1, -1,
            1, 1,
            -1, -1,
            -1, 1,
    };

    memcpy(vertexs, v, sizeof(v));
}

RectangleOpengl::~RectangleOpengl() {

}

void RectangleOpengl::onCreate() {
    program = createProgram(vertex, fragment, &vertexShader, &fragmentShader);
    LOGD("opengl program %d", program)
    //矩形
    vPosition = glGetAttribLocation(program, "a_position");
}

void RectangleOpengl::onChange(int width, int height) {
    BaseOpengl::onChange(width, height);
    glViewport(0, 0, width, height);
}

void RectangleOpengl::onDraw() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    //矩形
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 8, vertexs);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
