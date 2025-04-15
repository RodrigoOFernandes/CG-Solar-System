#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void multMatrixVector(float *m, float *v, float *res);


#endif
