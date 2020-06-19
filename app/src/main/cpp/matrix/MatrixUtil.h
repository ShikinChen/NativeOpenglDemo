//
// Created by Shiki on 2020/6/20.
//

#ifndef NATIVEOPENGLDEMO_MATRIXUTIL_H
#define NATIVEOPENGLDEMO_MATRIXUTIL_H

double getAngle(double angle);

#include <math.h>

static void initMatrix(float *matrix) {
    for (int i = 0; i < 16; ++i) {
        if (i % 5 == 0) {
            matrix[i] = 1;
        } else {
            matrix[i] = 0;
        }
    }
}

/**
 * 沿着Z轴旋转
 * @param matrix
 * @param angle
 */
static void rotateMatrixByZ(float *matrix, double angle) {
    angle = getAngle(angle);
    matrix[0] = cos(angle);
    matrix[1] = -sin(angle);

    matrix[4] = sin(angle);
    matrix[5] = cos(angle);

}

/**
 * 沿着Y轴旋转
 * @param matrix
 * @param angle
 */
static void rotateMatrixByY(float *matrix, double angle) {
    angle = getAngle(angle);
    matrix[0] = cos(angle);
    matrix[2] = sin(angle);


    matrix[8] = -sin(angle);
    matrix[10] = cos(angle);
}

/**
 * 沿着X轴旋转
 * @param matrix
 * @param angle
 */
static void rotateMatrixByX(float *matrix, double angle) {
    angle = getAngle(angle);
    matrix[5] = cos(angle);
    matrix[7] = -sin(angle);

    matrix[9] = sin(angle);
    matrix[10] = cos(angle);
}

/**
 * 缩放
 * @param matrix
 * @param angle
 */
static void scaleMatrix(float *matrix, double scaleX, double scaleY) {
    matrix[0] = scaleX;
    matrix[5] = scaleY;
}

/**
 * 等比缩放
 * @param matrix
 * @param angle
 */
static void scaleMatrix(float *matrix, double scale) {
    scaleMatrix(matrix, scale, scale);
}

static void transMatrix(float *matrix, double transX, double transY, double transZ = 0) {
    matrix[3] = transX;
    matrix[7] = transY;
    matrix[11] = transZ;
}

double getAngle(double angle) {
    angle = angle * M_PI / 180.0;
    return angle;
}


#endif //NATIVEOPENGLDEMO_MATRIXUTIL_H
