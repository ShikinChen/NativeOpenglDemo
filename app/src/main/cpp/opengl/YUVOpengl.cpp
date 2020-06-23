//
// Created by Shiki on 2020/6/22.
//

#include "YUVOpengl.h"

YUVOpengl::YUVOpengl() {

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
               "uniform sampler2D sampler_y;\n"
               "uniform sampler2D sampler_u;\n"
               "uniform sampler2D sampler_v;\n"
               "void main() {\n"
               "   float y,u,v;\n"
               "   y = texture2D(sampler_y,ft_Position).r;\n"
               "   u = texture2D(sampler_u,ft_Position).r - 0.5;\n"
               "   v = texture2D(sampler_v,ft_Position).r - 0.5;\n"
               "\n"
               "   vec3 rgb;\n"
               "   rgb.r = y + 1.403 * v;\n"
               "   rgb.g = y - 0.344 * u - 0.714 * v;\n"
               "   rgb.b = y + 1.770 * u;\n"
               "\n"
               "   gl_FragColor = vec4(rgb,1);\n"
               "}";
}

YUVOpengl::~YUVOpengl() {
    yuvHeight = 0;
    yuvWidth = 0;
    freeYUV();
}

void YUVOpengl::freeYUV() {
    if (y != NULL) {
        free(y);
        y = NULL;
    }
    if (u != NULL) {
        free(u);
        u = NULL;
    }
    if (v != NULL) {
        free(v);
        v = NULL;
    }
}

void YUVOpengl::onCreate() {
    program = createProgram(vertex, fragment, &vertexShader, &fragmentShader);
    vPosition = glGetAttribLocation(program, "v_Position");//顶点坐标
    fPosition = glGetAttribLocation(program, "f_Position");//纹理坐标
    samplerY = glGetUniformLocation(program, "sampler_y");
    samplerU = glGetUniformLocation(program, "sampler_u");
    samplerV = glGetUniformLocation(program, "sampler_v");
    uMatrix = glGetUniformLocation(program, "u_Matrix");

    glGenTextures(3, samplers);
    for (int i = 0; i < 3; ++i) {
        glBindTexture(GL_TEXTURE_2D, samplers[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //解绑纹理
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void YUVOpengl::onChange(int width, int height) {
    BaseOpengl::onChange(width, height);
    glViewport(0, 0, width, height);
    setOrthoM();
}

void YUVOpengl::onDraw() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glUniformMatrix4fv(uMatrix, 1, GL_FALSE, matrix);

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 8, vertexs);

    glEnableVertexAttribArray(fPosition);
    glVertexAttribPointer(fPosition, 2, GL_FLOAT, false, 8, fragments);

    if (yuvHeight > 0 && yuvWidth > 0) {
        if (y != NULL) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, samplers[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yuvWidth, yuvHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, y);
            glUniform1i(samplerY, 0);
        }
        if (u != NULL) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, samplers[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yuvWidth / 2, yuvHeight / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                         u);
            glUniform1i(samplerU, 1);
        }

        if (v != NULL) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, samplers[2]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yuvWidth / 2, yuvHeight / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                         v);
            glUniform1i(samplerV, 2);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void YUVOpengl::setOrthoM() {
    if (surfaceWidth <= 0 || surfaceHeight <= 0 || yuvWidth <= 0 || yuvHeight <= 0) {
        return;
    }
    initMatrix(matrix);

    float sreen_r = surfaceWidth * 1.0 / surfaceHeight;
    float img_r = yuvWidth * 1.0 / yuvHeight;

    if (sreen_r > img_r) {//宽度缩放
        float r = surfaceWidth / (surfaceHeight * 1.0 / yuvHeight * yuvWidth);
        orthoM(matrix, r, 1);
    } else {//高度缩放
        float r = surfaceHeight / (surfaceWidth * 1.0 / yuvWidth * yuvHeight);
        orthoM(matrix, 1, r);
    }
}

void YUVOpengl::setYuvData(void *y, void *u, void *v, int width, int height) {
    if (height > 0 && width > 0) {
        if (width != yuvWidth || height != yuvHeight) {
            yuvWidth = width;
            yuvHeight = height;
            freeYUV();
            this->y = malloc(yuvWidth * yuvHeight);
            this->u = malloc(yuvWidth * yuvHeight / 4);
            this->v = malloc(yuvWidth * yuvHeight / 4);
            setOrthoM();
        }

        memcpy(this->y, y, yuvHeight * yuvWidth);
        memcpy(this->u, u, yuvHeight * yuvWidth / 4);
        memcpy(this->v, v, yuvHeight * yuvWidth / 4);
    }
}

void YUVOpengl::destroy() {
    glDeleteTextures(3, samplers);
    BaseOpengl::destroy();
}
