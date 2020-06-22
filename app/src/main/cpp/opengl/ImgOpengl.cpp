//
// Created by Shiki on 2020/6/21.
//

#include "ImgOpengl.h"

ImgOpengl::ImgOpengl() {

}

ImgOpengl::~ImgOpengl() {
    pixel = NULL;
}

void ImgOpengl::onCreate() {
    vertex = "attribute vec4 v_Position;\n"
             "attribute vec2 f_Position;\n"
             "varying vec2 ft_Position;\n"
             "uniform mat4 u_Matrix;\n"
             "void main() {\n"
             "    ft_Position = f_Position;\n"
             "    gl_Position = v_Position * u_Matrix;\n"
             "}";
    fragment = "precision mediump float;\n"
               "varying vec2 ft_Position;\n"
               "uniform sampler2D sTexture;\n"
               "void main() {\n"
               "    gl_FragColor=texture2D(sTexture, ft_Position);\n"
               "}";

    program = createProgram(vertex, fragment);
    LOGD("opengl program %d", program)


//        rotateMatrixByZ(matrix, 60);
//        scaleMatrix(matrix, 3.5);
//        transMatrix(matrix, 1, -1);

    v_Position = glGetAttribLocation(program, "v_Position");//顶点坐标
    f_Position = glGetAttribLocation(program, "f_Position");//纹理坐标
    sTexture = glGetUniformLocation(program, "sTexture");//2D纹理
    u_Matrix = glGetUniformLocation(program, "u_Matrix");

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    //解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImgOpengl::onChange(int width, int height) {
    glViewport(0, 0, width, height);
    setOrthoM();
}

void ImgOpengl::onDraw() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, matrix);


    //绑定第五个纹理
    glActiveTexture(GL_TEXTURE5);
    glUniform1i(sTexture, 5);


    glBindTexture(GL_TEXTURE_2D, textureId);

    if (pixel != NULL) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    }

    glEnableVertexAttribArray(v_Position);
    glVertexAttribPointer(v_Position, 2, GL_FLOAT, false, 8, vertexs);

    glEnableVertexAttribArray(f_Position);
    glVertexAttribPointer(f_Position, 2, GL_FLOAT, false, 8, fragments);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImgOpengl::setOrthoM() {
    if (surfaceWidth <= 0 || surfaceHeight <= 0 || width <= 0 || height <= 0) {
        return;
    }
    initMatrix(matrix);

    float sreen_r = surfaceWidth * 1.0 / surfaceHeight;
    float img_r = width * 1.0 / height;

    if (sreen_r > img_r) {//宽度缩放
        float r = surfaceWidth / (surfaceHeight * 1.0 / height * width);
        orthoM(matrix, r, 1);
    } else {//高度缩放
        float r = surfaceHeight / (surfaceWidth * 1.0 / width * height);
        orthoM(matrix, 1, r);
    }
}

void ImgOpengl::setPixel(void *data, int width, int height) {
    pixel = data;
    this->width = width;
    this->height = height;
    setOrthoM();
}
