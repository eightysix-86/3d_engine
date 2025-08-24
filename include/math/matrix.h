#include <math.h>

#include "./vector.h"

typedef enum { X, Y, Z } Axis;

typedef struct Matrix
{
    float m[4][4];
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

/**
 * @brief Creates a 4x4 translation matrix for 3D transformations
 *
 * @param tx The translation along the x-axis
 * @param ty The translation along the y-axis
 * @param tz The translation along the z-axis
 *
 * @return Matrix A 4x4 matrix representing the translation transformation
 *
 * The resulting matrix will be of the form:
 * | 1  0  0  tx |
 * | 0  1  0  ty |
 * | 0  0  1  tz |
 * | 0  0  0  1  |
 */
Matrix translation_matrix(float tx, float ty, float tz);

/**
 * @brief Creates a 4x4 scaling transformation matrix
 *
 * Creates a matrix that can be used to scale 3D objects by different factors along each axis.
 * The scaling is performed relative to the origin (0,0,0).
 *
 * @param sx Scale factor along the x-axis
 * @param sy Scale factor along the y-axis
 * @param sz Scale factor along the z-axis
 *
 * @return Matrix A 4x4 matrix containing the scaling transformation
 *
 * @note The resulting matrix will scale points according to:
 *       x' = sx * x
 *       y' = sy * y
 *       z' = sz * z
 */
Matrix scaling_matrix(float sx, float sy, float sz);

/**
 * @brief Creates a 3D rotation matrix around a specified axis
 * @param theta The angle of rotation in radians
 * @param axis The axis of rotation (X, Y, or Z)
 * @return Matrix A 4x4 rotation matrix
 *
 * Creates a rotation matrix that can be used to rotate points or vectors
 * around the specified axis by the given angle theta.
 * The rotation follows the right-hand rule convention.
 */
Matrix rotation_matrix(float theta, Axis axis);
