#include <math.h>

#include "test_framework.h"
#include "core/transform.h"

#define TOTAL_TESTS 5

int main(void) {
    Vector4 v = {1, 0, 0, 1};   // point at (1,0,0)
    float theta = M_PI / 2;

    TestResult results[TOTAL_TESTS];

    // Rotate around Z
    Matrix rz = rotation_matrix(theta, Z);
    run_test("Rotation around Z",
        transform(rz, v),
        ((Vector4){0, 1, 0, 1}),
        &results[0]);

    // Translate by (1,2,3)
    Matrix t = translation_matrix(1, 2, 3);
    run_test("Translation",
        transform(t, v),
        ((Vector4){2, 2, 3, 1}),
        &results[1]);

    // Scale by (2,2,2)
    Matrix s = scaling_matrix(2, 2, 2);
    run_test("Scaling",
        transform(s, v),
        ((Vector4){2, 0, 0, 1}),
        &results[2]);

    // Combined T * R * S
    Matrix r = rotation_matrix(theta, X);
    Matrix trs = multiply(t, multiply(r, s));
    run_test_matrix("Combined TRS", 
        trs,
       ((Matrix){{
            {2, 0, 0, 1},
            {0, 0, -2, 2},
            {0, 2, 0, 3},
            {0, 0, 0, 1}
        }}), 
        &results[3]);

    // T * R * S * v
    run_test("Transformation TRS",
        transform(trs, v),
        ((Vector4){3, 2, 3, 1}),
        &results[4]);

    print_summary(results, TOTAL_TESTS);
}
