#pragma once
#include <stddef.h>


double *matrixd(size_t rows, size_t cols);

void matd_print(double *m, size_t rows, size_t cols);

double *matd_transpose(double *m, size_t rows, size_t cols);

double *matd_add(double *m1, double *m2, size_t rows, size_t cols);

double *matd_mul(double *m1, double *m2, size_t r1, size_t c1, size_t c2);

double matd_get(double *mat, size_t rows, size_t cols, size_t x, size_t y);

void matd_set(double *mat, size_t rows, size_t cols, size_t x, size_t y, double value);

void matd_free(double *mat);

