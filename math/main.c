#include "matrix.h"
#include "sigmoid.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

int test_matrixd() {
    printf("Testing matrix.c...\n");
    size_t rows_a = 2, cols_a = 3, cols_b = 2;

    double *a = matrixd(rows_a, cols_a); // 2*3 Matrix
    double *b = matrixd(cols_a, cols_b); // 3*2 Matrix

    // Fill Matrix a
    matd_set(a, rows_a, cols_a, 0, 0, 1.0);
    matd_set(a, rows_a, cols_a, 1, 0, 2.0);
    matd_set(a, rows_a, cols_a, 2, 0, 3.0);
    matd_set(a, rows_a, cols_a, 0, 1, 4.0);
    matd_set(a, rows_a, cols_a, 1, 1, 5.0);
    matd_set(a, rows_a, cols_a, 2, 1, 6.0);

    // Fill Matrix b
    matd_set(b, cols_a, cols_b, 0, 0, 7.0);
    matd_set(b, cols_a, cols_b, 1, 0, 8.0);
    matd_set(b, cols_a, cols_b, 0, 1, 9.0);
    matd_set(b, cols_a, cols_b, 1, 1, 10.0);
    matd_set(b, cols_a, cols_b, 0, 2, 11.0);
    matd_set(b, cols_a, cols_b, 1, 2, 12.0);

    // Multiply a and b
    double *result = matd_mul(a, b, rows_a, cols_a, cols_b);

    // Check the result
    assert(matd_get(result, rows_a, cols_b, 0, 0) == 58.0);
    assert(matd_get(result, rows_a, cols_b, 1, 0) == 64.0);
    assert(matd_get(result, rows_a, cols_b, 0, 1) == 139.0);
    assert(matd_get(result, rows_a, cols_b, 1, 1) == 154.0);

    // Free the matrices
    matd_free(a);
    matd_free(b);
    matd_free(result);

    printf("All tests passed!\n");
    return 0;
}

void test_sigmoid() {
    double value = 1;
    printf("Sigmoid of %6f:\n", value);
    printf("\tExpected: %6f\n", 0.731059);
    printf("\tActual  : %6f\n", sigmoid(value));


    value = 0;
    printf("Sigmoid of %6f:\n", value);
    printf("\tExpected: %6f\n", 0.5);
    printf("\tActual  : %6f\n", sigmoid(value));

    printf("\n");
}

void test_prime_sigmoid() {
    double value = 1;
    printf("Prime sigmoid of %6f:\n", value);
    printf("\tExpected: %6f\n", 0.196612);
    printf("\tActual  : %6f\n", prime_sigmoid(value));


    value = 0;
    printf("Prime sigmoid of %6f:\n", value);
    printf("\tExpected: %6f\n", 0.25);
    printf("\tActual  : %6f\n", prime_sigmoid(value));

    printf("\n");
}

int main() {
    test_sigmoid();
    test_prime_sigmoid();

    return test_matrixd();
}
