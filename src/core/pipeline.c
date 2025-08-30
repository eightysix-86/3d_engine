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
    
    Vector3 z = subtract(pos, target);
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
        {cam.x.x, cam.y.x, - cam.z.x, 0},
        {cam.x.y, cam.y.y, - cam.z.y, 0},
        {cam.x.z, cam.y.z, - cam.z.z, 0},
        {dot(neg(cam.x), camera.pos), dot(neg(cam.y), camera.pos), dot(cam.z, camera.pos), 1}
    }};
}

/* **************************** PROJECTION ****************************** */

// Perspective projection
static Matrix projection_matrix(Projection proj) {
    float tan_fov = tan(proj.fov / 2);
    float a = 1 / (proj.aspect_ratio * tan_fov);
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

void update_mesh(Mesh* figure, const Transform transformations, const Camera cam, const Projection proj) {
    Matrix m = model_matrix(transformations);
    Matrix v = view_matrix(cam);
    Matrix p = projection_matrix(proj);
    
    Matrix mvp = multiply(m, multiply(v, p));

    for (size_t i = 0; i < figure->vertex_count; i++)
        figure->vertices[i] = transform(mvp, figure->vertices[i]);

}