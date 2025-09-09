#include "core/transform.h"

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
                     {0, cosf(theta), -sinf(theta), 0},
                     {0, sinf(theta), cosf(theta), 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    case Y:
    {
        Matrix m = {{{cosf(theta), 0, sinf(theta), 0},
                     {0, 1, 0, 0},
                     {-sinf(theta), 0, cosf(theta), 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    case Z:
    {
        Matrix m = {{{cosf(theta), -sinf(theta), 0, 0},
                     {sinf(theta), cosf(theta), 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    }
}
