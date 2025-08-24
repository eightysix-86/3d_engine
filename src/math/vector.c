#include "../include/math/vector.h"

Vector4 add(Vector4 v1, Vector4 v2) {
    Vector4 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;
    return result;
}

Vector4 subtract(Vector4 v1, Vector4 v2) {
    Vector4 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    result.w = v1.w - v2.w;
    return result;
}

float dot(Vector4 v1, Vector4 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

Vector4 array_to_vect(float arr[4]) {
    Vector4 result = {arr[0], arr[1], arr[2], arr[3]};
    return result;
}
