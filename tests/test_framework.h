#include "math/matrix.h"

typedef struct {
    const char* name;
    int passed;
} TestResult;

void run_test_float(const char* name, float got, float expected, TestResult* result);

void run_test_vector3(const char* name, Vector3 got, Vector3 expected, TestResult* result);

void run_test_vector4(const char* name, Vector4 got, Vector4 expected, TestResult* result);

void run_test_matrix(const char* name, Matrix got, Matrix expected, TestResult* result);

void print_summary(TestResult* results, int total);

#define run_test(name, got, expected, result) \
    _Generic((got), \
        float: run_test_float, \
        Vector3: run_test_vector3, \
        Vector4: run_test_vector4, \
        Matrix: run_test_matrix \
    )(name, got, expected, result)
