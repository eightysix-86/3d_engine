#include <math.h>

#include "core/pipeline.h"

/* **************************** INIT -> MODEL ****************************** */
static Matrix model_matrix(Transform transform) {
    Vector3 trans_v = transform.translation;
    Vector3 scale_v = transform.scale;
    Vector3 rot_v = transform.rotation;

    Matrix trs = translation_matrix(trans_v.x, trans_v.y, trans_v.z);

    if (rot_v.z != 0)
        trs = multiply(trs, rotation_matrix(rot_v.z, Z));
    if (rot_v.y != 0)
        trs = multiply(trs, rotation_matrix(rot_v.y, Y));
    if (rot_v.x != 0)
        trs = multiply(trs, rotation_matrix(rot_v.x, X));

    trs = multiply(trs, scaling_matrix(scale_v.x, scale_v.y, scale_v.z));

    return trs;
}

/* **************************** WORLD --> VIEW ****************************** */

// lookAt/Gram–Schmidt camera rotation
static CameraAxes get_camera(const Vector3 pos, const Vector3 target, const Vector3 up) {
    CameraAxes camera;
    
    Vector3 z = subtract(target, pos);
    camera.z = normalize(z);

    Vector3 x = cross(up, camera.z);
    camera.x = normalize(x);

    camera.y = cross(camera.z, camera.x);

    return camera;
}

// world -> camera (inverse of camera -> world)
static Matrix view_matrix(const Camera camera) {
    CameraAxes cam = get_camera(camera.pos, camera.target, camera.up);

    return (Matrix) {{
        {cam.x.x, cam.x.y, cam.x.z, -dot(cam.x, camera.pos)},
        {cam.y.x, cam.y.y, cam.y.z, -dot(cam.y, camera.pos)},
        {cam.z.x, cam.z.y,cam.z.z, dot(cam.z, camera.pos)},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
}

/* **************************** PROJECTION ****************************** */

// Perspective projection
static Matrix projection_matrix(Projection proj) {
    float tan_fov = tanf(proj.fov / 2);
    float a = 1.0f / (proj.aspect_ratio * tan_fov);
    float b = (proj.far + proj.near) / (proj.near - proj.far);
    float c = (2 * proj.far * proj.near) / (proj.near - proj.far);

    return (Matrix) {{
        {a, 0, 0, 0},
        {0, 1 / tan_fov, 0, 0},
        {0, 0, b, c},
        {0, 0, -1, 0}
    }};
}

/* ****************************  MODEL + VIEW + PROJ ****************************** */

void update_mesh(Mesh* figure, Mesh* clipped, const Transform transformations, const Camera cam, const Projection proj) {
    Matrix m = model_matrix(transformations);
    Matrix v = view_matrix(cam);
    Matrix p = projection_matrix(proj);
    
    Matrix mvp = multiply(p, multiply(v, m));

    for (size_t i = 0; i < figure->vertex_count; i++)
        clipped->vertices[i] = transform(mvp, figure->vertices[i]);
}
