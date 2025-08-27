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
                     {0, cos(theta), -sin(theta), 0},
                     {0, sin(theta), cos(theta), 0},
                     {0, 0, 0, 1}}};
        return m;
    }
    case Y:
    {
        Matrix m = {{{cos(theta), 0, sin(theta), 0},
                     {0, 1, 0, 0},
                     {-sin(theta), 0, cos(theta), 0},
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
