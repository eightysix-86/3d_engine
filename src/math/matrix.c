#include <stdio.h>
#include <assert.h>

#include "math/matrix.h"

Vector4 transform(Matrix M, Vector4 v)
{
    Vector4 result;
    result.x = dot(array_to_vect(M.m[0]), v);
    result.y = dot(array_to_vect(M.m[1]), v);
    result.z = dot(array_to_vect(M.m[2]), v);
    result.w = dot(array_to_vect(M.m[3]), v);
    return result;
}

Matrix multiply(Matrix A, Matrix B) {
    Matrix C;
    for (size_t i = 0; i < MATRIX_N; i++) {
        for (size_t j = 0; j < MATRIX_N; j++) {
            C.m[i][j] = dot(array_to_vect(A.m[i]), extract_column(&B, j));
        }
    }
    return C;
}

Vector4 extract_column(const Matrix *M, size_t j) {
    assert(j < MATRIX_N);
    return (Vector4){ M->m[0][j], M->m[1][j], M->m[2][j], M->m[3][j] };
}
