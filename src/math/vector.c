#pragma once

#include "math/vector.h"

inline Vector4 add(Vector4 v1, Vector4 v2) {
    return (Vector4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline Vector4 subtract(Vector4 v1, Vector4 v2) {
    return (Vector4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline float dot(Vector4 v1, Vector4 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

inline Vector4 array_to_vect(float arr[4]) {
    return (Vector4){arr[0], arr[1], arr[2], arr[3]};
}
