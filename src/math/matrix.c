#include <../include/math/matrix.h>

Vector4 transform(Matrix M, Vector4 v)
{
    Vector4 result;
    result.x = dot(array_to_vect(M.m[0]), v);
    result.y = dot(array_to_vect(M.m[1]), v);
    result.z = dot(array_to_vect(M.m[2]), v);
    result.w = dot(array_to_vect(M.m[3]), v);
    return result;
}

Matrix translation_matrix(float tx, float ty, float tz)
{
    Matrix m = {{{1, 0, 0, tx},
                 {0, 1, 0, ty},
                 {0, 0, 1, tz},
                 {0, 0, 0, 1}}};
    return m;
}

Matrix scaling_matrix(float sx, float sy, float sz)
{
    Matrix m = {{{sx, 0, 0, 0},
                 {0, sy, 0, 0},
                 {0, 0, sz, 0},
                 {0, 0, 0, 1}}};
    return m;
}

Matrix rotation_matrix(float theta, Axis axis)
{
    switch (axis)
    {
    case X:
    {
        Matrix m = {{{1, 0, 0, 0},
                     {0, cos(theta), sin(theta), 0},
                     {0, -sin(theta), cos(theta), 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    case Y:
    {
        Matrix m = {{{cos(theta), 0, -sin(theta), 0},
                     {0, 1, 0, 0},
                     {sin(theta), 0, cos(theta), 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    case Z:
    {
        Matrix m = {{{cos(theta), -sin(theta), 0, 0},
                     {sin(theta), cos(theta), 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    }
}
