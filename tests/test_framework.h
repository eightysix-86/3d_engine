#include "math/matrix.h"

typedef struct {
    const char* name;
    int passed;
} TestResult;

// Runs a test (assert-like but doesn’t stop program)
void run_test_vector(const char* name, Vector4 got, Vector4 expected, TestResult* result);
void run_test_matrix(const char* name, Matrix got, Matrix expected, TestResult* result);

void print_summary(TestResult* results, int total);
