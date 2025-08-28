#pragma once

#include "math.h"

/**
 * @brief A 4D vector type for homogeneous coordinates in 3D graphics
 *
 * This type represents a 4D vector using homogeneous coordinates,
 * typically for positions or directions in 3D space.
 *
 * It provides two access modes:
 * - Named components: .x, .y, .z, .w
 * - Array indexing: v[0..3]
 *
 * Usage:
 *   Vector4 p = { .x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 1.0f }; // point
 *   Vector4 d = { .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f }; // direction
 *
 * @field x The x component
 * @field y The y component
 * @field z The z component
 * @field w The w component (1 for points, 0 for directions)
 * @field v Alternative array access (v[0]=x, v[1]=y, v[2]=z, v[3]=w)
 */
typedef union {
    struct { float x, y, z, w; };
    float v[4];
} Vector4;

typedef union {
    struct { float x, y, z; };
    float v[3];
} Vector3;

static inline Vector4 add4(Vector4 v1, Vector4 v2) {
    return (Vector4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

static inline Vector3 add3(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

static inline Vector4 subtract4(Vector4 v1, Vector4 v2) {
    return (Vector4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

static inline Vector3 subtract3(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

static inline Vector3 cross(Vector3 v1, Vector3 v2) {
    return (Vector3) {(v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x)};
}

static inline float dot4(Vector4 v1, Vector4 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

static inline float dot3(Vector3 v1, Vector3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

static inline float norm4(Vector4 v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
}

static inline float norm3(Vector3 v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

inline Vector4 array_to_vect(float arr[4]) {
    return (Vector4){arr[0], arr[1], arr[2], arr[3]};
}

#define add(a, b) _Generic((a), \
    Vector3: add3, \
    Vector4: add4 \
)(a, b)

#define subtract(a, b) _Generic((a), \
    Vector3: subtract3, \
    Vector4: subtract4 \
)(a, b)

#define dot(a, b) _Generic((a), \
    Vector3: dot3, \
    Vector4: dot4 \
)(a, b)

#define norm(a) _Generic((a), \
    Vector3: norm3, \
    Vector4: norm4 \
)(a)
