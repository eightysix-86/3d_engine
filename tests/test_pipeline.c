#include <math.h>

#include "test_framework.h"
#include "core/pipeline.h"

#define TOTAL_TESTS 8

#define FOV (M_PI / 2)
#define NEAR_PLANE 0.2f
#define FAR_PLANE 10000.0f
#define ASPECT_RATIO 1.0f
#define CAM_POS (Vector3){0.0f, 0.0f, 10.0f}
#define CAM_TARGET (Vector3){0.0f, 0.0f, 0.0f}
#define CAM_UP (Vector3){0.0f, 1.0f, 0.0f}

static inline Mesh* got_value(Mesh* original, Transform transform, Camera cam, Projection proj) {
    Mesh* copy = mesh_copy(original);
    update_mesh(original, copy, transform, cam, proj);
    return copy;
}

int main(void) {
    float theta = M_PI/2;

    Camera cam = {
        .pos    = CAM_POS,
        .target = CAM_TARGET,
        .up     = CAM_UP
    };

    Projection proj = {
        .fov          = FOV,
        .aspect_ratio = ASPECT_RATIO,
        .near         = NEAR_PLANE,
        .far          = FAR_PLANE
    };

    // ------------------------------ Mesh initial ------------------------------ 

    Mesh* single_point_before = mesh_generate((Vector4[]){ {1.0f, 0.0f, 0.0f, 1.0f} }, 1, NULL, 0);

    Vector4 f1_vertices[8] = {
        {0, 0, 0, 1},  // 0
        {1, 0, 0, 1},  // 1
        {0, 1, 0, 1},  // 2
    };
    Triangle f1_triangles[12] = {
        {0, 1, 2}
    };
    Mesh* triangle_before = mesh_generate(f1_vertices, 3, f1_triangles, 1);

    // ------------------------------ Transformations ------------------------------

    Transform no_transform = {
        .translation = {0.0f, 0.0f, 0.0f},  
        .scale       = {1.0f, 1.0f, 1.0f},  
        .rotation    = {0.0f, 0.0f, 0.0f} 
    };

    Transform rotationY = {
        .translation = {0.0f, 0.0f, 0.0f},  
        .scale       = {1.0f, 1.0f, 1.0f},  
        .rotation    = {0.0f, theta, 0.0f}
    };

    Transform scale = {
        .translation = {0.0f, 0.0f, 0.0f},  
        .scale       = {2.0f, 2.0f, 2.0f},  
        .rotation    = {0.0f, 0.0f, 0.0f} 
    };

    Transform rotationZ_and_translationX = {
        .translation = {1.0f, 0.0f, 0.0f},  
        .scale       = {1.0f, 1.0f, 1.0f},  
        .rotation    = {0.0f, 0.0f, theta}
    };

    // ------------------------------ Expected Mesh ------------------------------

    // Single point transformations
    Mesh* single_point_r = mesh_generate(
        (Vector4[]){ {0, 0, -1, 1} }, 1, NULL, 0
    );

    Mesh* single_point_s = mesh_generate(
        (Vector4[]){ {2, 0, 0, 1} }, 1, NULL, 0
    );

    Mesh* single_point_rt = mesh_generate(
        (Vector4[]){ {1, 1, 0, 1} }, 1, NULL, 0
    );

    // Triangle transformations
    Mesh* triangle_r = mesh_generate(
        (Vector4[]){
            {0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, -1, 1}
        }, 3,
        (Triangle[]){ {0,1,2} }, 1
    );

    Mesh* triangle_s = mesh_generate(
        (Vector4[]){
            {0, 0, 0, 1},
            {2, 0, 0, 1},
            {0, 2, 0, 1}
        }, 3,
        (Triangle[]){ {0,1,2} }, 1
    );

    Mesh* triangle_rt = mesh_generate(
        (Vector4[]){
            {1, 0, 0, 1},
            {1, 1, 0, 1},
            {0, 0, 0, 1}
        }, 3,
        (Triangle[]){ {0,1,2} }, 1
    );

    // ------------------------------ Got Mesh ------------------------------
    Mesh* single_point_got = got_value(single_point_before, no_transform, cam, proj);
    Mesh* single_point_r_got = got_value(single_point_before, rotationY, cam, proj);
    Mesh* single_point_s_got = got_value(single_point_before, scale, cam, proj);
    Mesh* single_point_rt_got = got_value(single_point_before, rotationZ_and_translationX, cam, proj);
    
    Mesh* triangle_got = got_value(triangle_before, no_transform, cam, proj);
    Mesh* triangle_r_got = got_value(triangle_before, rotationY, cam, proj);
    Mesh* triangle_s_got = got_value(triangle_before, scale, cam, proj);
    Mesh* triangle_rt_got = got_value(triangle_before, rotationZ_and_translationX, cam, proj);

    // ------------------------------ Tests ------------------------------
    TestResult results[TOTAL_TESTS];
    
    run_test("1. No transformations on single point",
        single_point_got,
        single_point_before,
        &results[0]);

    run_test("2. Rotation Y on single point",
        single_point_r_got,
        single_point_r,
        &results[1]);
    
    run_test("3. Scaling on single point",
        single_point_s_got,
        single_point_s,
        &results[2]);
    
    run_test("4. Rotation Z + translation on single point",
        single_point_rt_got,
        single_point_rt,
        &results[3]);
    
    run_test("5. No transformations on triangle",
        triangle_got,
        triangle_before,
        &results[4]);

    run_test("6. Rotation Y on triangle",
        triangle_r_got,
        triangle_r,
        &results[5]);
    
    run_test("7. Scaling on triangle",
        triangle_s_got,
        triangle_s,
        &results[6]);
    
    run_test("8. Rotation Z + translation on triangle",
        triangle_rt_got,
        triangle_rt,
        &results[7]);

    print_summary(results, TOTAL_TESTS);

    // ------------------------------ Garbage collect ------------------------------
    mesh_destroy(single_point_before);
    mesh_destroy(single_point_got);
    mesh_destroy(single_point_r);
    mesh_destroy(single_point_r_got);
    mesh_destroy(single_point_rt);
    mesh_destroy(single_point_rt_got);
    mesh_destroy(single_point_s);
    mesh_destroy(single_point_s_got);
    mesh_destroy(triangle_before);
    mesh_destroy(triangle_got);
    mesh_destroy(triangle_r);
    mesh_destroy(triangle_r_got);
    mesh_destroy(triangle_rt);
    mesh_destroy(triangle_rt_got);
    mesh_destroy(triangle_s);
    mesh_destroy(triangle_s_got);
}
