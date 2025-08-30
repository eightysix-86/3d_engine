#include "math/matrix.h"
#include "core/transform.h"
#include "core/mesh.h"

typedef struct Camera {
    Vector3 pos, target, up;
} Camera;

typedef struct CameraAxes {
    Vector3 x, y, z;
} CameraAxes;

typedef struct Projection {
    float fov, aspect_ratio, near, far;
} Projection;

void update_mesh(Mesh* figure, Transform transformations, Camera cam, Projection proj);
