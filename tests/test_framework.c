#include <stdio.h>
#include <math.h>

#include "test_framework.h"

#define EPSILON 1e-5f

static int almost_equal(float a, float b) {
    return fabsf(a - b) < EPSILON;
}

void run_test_float(const char* name, float got, float expected, TestResult* result){
    if (almost_equal(got, expected)) {
        printf("✅ %s passed\n", name);
        result->passed = 1;
    } else {
        printf("❌ %s failed!\n", name);
        printf("   Got      = %.5f\n", got);
        printf("   Expected = %.5f\n", expected);
        result->passed = 0;
    }
    result->name = name;
}

void run_test_vector3(const char* name, Vector3 got, Vector3 expected, TestResult* result) {
    if (almost_equal(got.x, expected.x) &&
        almost_equal(got.y, expected.y) &&
        almost_equal(got.z, expected.z)) {
        printf("✅ %s passed\n", name);
        result->passed = 1;
    } else {
        printf("❌ %s failed!\n", name);
        printf("   Got      = (%.5f, %.5f, %.5f)\n", got.x, got.y, got.z);
        printf("   Expected = (%.5f, %.5f, %.5f)\n", expected.x, expected.y, expected.z);
        result->passed = 0;
    }
    result->name = name;
}

void run_test_vector4(const char* name, Vector4 got, Vector4 expected, TestResult* result) {
    if (almost_equal(got.x, expected.x) &&
        almost_equal(got.y, expected.y) &&
        almost_equal(got.z, expected.z) &&
        almost_equal(got.w, expected.w)) {
        printf("✅ %s passed\n", name);
        result->passed = 1;
    } else {
        printf("❌ %s failed!\n", name);
        printf("   Got      = (%.5f, %.5f, %.5f, %.5f)\n", got.x, got.y, got.z, got.w);
        printf("   Expected = (%.5f, %.5f, %.5f, %.5f)\n", expected.x, expected.y, expected.z, expected.w);
        result->passed = 0;
    }
    result->name = name;
}

void run_test_matrix(const char* name, Matrix got, Matrix expected, TestResult* result) {
    int ok = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!almost_equal(got.m[i][j], expected.m[i][j])) {
                ok = 0;
            }
        }
    }
    if (ok) {
        printf("✅ %s passed\n", name);
        result->passed = 1;
    } else {
        printf("❌ %s failed!\n", name);
        result->passed = 0;
    }
    result->name = name;
}

void run_test_mesh(const char* name, Mesh* got, Mesh* expected, TestResult* result) {
    printf("%s\n", name);
    result->passed = 1;
    int temp = 1;

    // Test vertices
    puts("  Test vertices");
    for (size_t i = 0; i < expected->vertex_count; i++) {
        if (got->vertices[i].w <= 0 ||
            got->vertices[i].x < -got->vertices[i].w || got->vertices[i].x > got->vertices[i].w ||
            got->vertices[i].y < -got->vertices[i].w || got->vertices[i].y > got->vertices[i].w ||
            got->vertices[i].z < -got->vertices[i].w || got->vertices[i].z > got->vertices[i].w) {
            printf("    ❌ Vertex %zu failed\n", i);
            printf("        Got      = (%.5f, %.5f, %.5f, %.5f)\n", got->vertices[i].x, got->vertices[i].y, got->vertices[i].z, got->vertices[i].w);
            result->passed = 0;
            temp = 0;
        }
    }

    if (temp)
        puts("    ✅ Vertices passed");

    temp = 1;
    // Test triangles
    puts("  Test triangles");
    for (size_t i = 0; i < expected->triangle_count; i++) {
        if ((got->triangles[i].vert[0] != expected->triangles[i].vert[0]) ||
            (got->triangles[i].vert[1] != expected->triangles[i].vert[1]) ||
            (got->triangles[i].vert[2] != expected->triangles[i].vert[2])) {
            printf("    ❌ Triangle %zu failed\n", i);
            printf("        Got      = (%d, %d, %d)\n", got->triangles[i].vert[0], got->triangles[i].vert[1], got->triangles[i].vert[2]);
            printf("        Expected = (%d, %d, %d)\n", expected->triangles[i].vert[0], expected->triangles[i].vert[1], expected->triangles[i].vert[2]);
            result->passed = 0;
            temp = 0;
        }
    }

    if (temp)
        puts("    ✅ Triangles passed");

    result->name = name;
}

void print_summary_f(const char* filename, TestResult* results, int total) {
    int passed = 0;
    for (int i = 0; i < total; i++) {
        if (results[i].passed) passed++;
    }

    printf("\n=== TEST SUMMARY (%s) ===\n", filename);
    printf("Passed %d/%d tests\n\n", passed, total);
}
