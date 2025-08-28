#include <math.h>

#include "test_framework.h"
#include "math/vector.h"

#define TOTAL_TESTS 6

int main(void) {
    Vector3 u1 = {3, 4, 5};
    Vector3 u2 = {7, 8, 9};
    Vector4 v = {1, 0, 0, -1};

    float u1_n = 5 * sqrt(2);
    float u2_n = sqrt(194);
    float v_n = sqrt(2);

    TestResult results[TOTAL_TESTS];

    // Cross product
    run_test("Cross product",
        cross(u1, u2),
        ((Vector3){-4, 8, -4}),
        &results[0]);

    // Norm 3D 1)
    run_test("Norm with Vector3 1)",
        norm(u1),
        u1_n,
        &results[1]);

    // Norm 3D 2)
    run_test("Norm with Vector3 2)",
        norm(u2),
        u2_n,
        &results[2]);
        
    // Norm 4D
    run_test("Norm with Vector4",
        norm(v),
        v_n,
        &results[3]);

    // Normalize 3D vector 1)
    run_test("Normalize 3D vector 1)",
        normalize(u1),
        ((Vector3){3 / u1_n, 4 / u1_n, 5 /u1_n}),
        &results[4]);    
    
    // Normalize 3D vector 2)
    run_test("Normalize 3D vector 2)",
        normalize(u2),
        ((Vector3){7 / u2_n, 8 / u2_n, 9 /u2_n}),
        &results[5]);

    // Normalize 4D vector
    run_test("Normalize 4D vector",
        normalize(v),
        ((Vector4){1 / v_n, 0, 0, -1 / v_n}),
        &results[6]);

    print_summary(results, TOTAL_TESTS);
}
