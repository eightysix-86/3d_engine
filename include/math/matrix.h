#pragma once

#include <math.h>
#include <stdio.h>

#include "math/vector.h"

#define MATRIX_N 4

typedef struct Matrix
{
    float m[MATRIX_N][MATRIX_N];
} Matrix;

/**
 * @brief Applies a matrix transformation to a 4D vector
 * @param M The transformation matrix to be applied
 * @param v The 4D vector to transform
 * @return A new Vector4 containing the transformed coordinates
 *
 * This function performs matrix multiplication M * v to transform
 * the vector v by matrix M. The transformation can represent various
 * operations such as rotation, translation, scaling, or any combination
 * of these.
 */
Vector4 transform(Matrix M, Vector4 v);

Matrix multiply(Matrix A, Matrix B);

Vector4 extract_column(const Matrix *M, size_t j);
