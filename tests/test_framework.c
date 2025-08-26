#include <stdio.h>
#include <math.h>

#include "test_framework.h"

#define EPSILON 1e-5f

static int almost_equal(float a, float b) {
    return fabsf(a - b) < EPSILON;
}

void run_test_vector(const char* name, Vector4 got, Vector4 expected, TestResult* result) {
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

void print_summary(TestResult* results, int total) {
    int passed = 0;
    for (int i = 0; i < total; i++) {
        if (results[i].passed) passed++;
    }
    printf("\n=== TEST SUMMARY ===\n");
    printf("Passed %d/%d tests\n", passed, total);
}
