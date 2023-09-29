#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <err.h>

char *MALLOC_FAILURE = "Memory allocation failed.";
char *NULL_POINTER_FAILURE = "Null reference exception.";

double *matrixd(size_t rows, size_t cols) {
    double *mat = malloc(rows * cols * sizeof(double)) ;

    if (mat == NULL) {
        errx(EXIT_FAILURE, "%s", MALLOC_FAILURE);
        return NULL;
    }

    for (size_t i = 0; i < rows * cols; i++) {
        *(mat + i) = 0;
    }

    return mat;
}

void matd_print(double *m, size_t rows, size_t cols)
{
    if (m == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return;
    }

    for (size_t y = 0; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            printf("%4g", m[cols * y + x]);
        }
        printf("\n");
    }

    printf("\n");
}

double *matd_transpose(double *m, size_t rows, size_t cols)
{
    if (m == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return NULL;
    }

    double *r = malloc(rows * cols * sizeof(double));

    if (r == NULL) {
        errx(EXIT_FAILURE, "%s", MALLOC_FAILURE);
        return NULL;
    }

    for (size_t y = 0; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            r[x * rows + y] = m[y * cols + x];
        }
    }

    return r;
}

double *matd_add(double *m1, double *m2, size_t rows, size_t cols)
{
    if (m1 == NULL || m2 == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return NULL;
    }

    double *r = malloc(rows * cols * sizeof(double));

    if (r == NULL) {
        errx(EXIT_FAILURE, "%s", MALLOC_FAILURE);
        return NULL;
    }


    for (size_t x = 0; x < cols; x++) {
        for (size_t y = 0; y < rows; y++) {
            r[y * cols + x] = m1[y * cols + x] + m2[y * cols + x];
        }
    }

    return r;
}

double *matd_sub(double *m1, double *m2, size_t rows, size_t cols)
{
    if (m1 == NULL || m2 == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return NULL;
    }

    double *r = malloc(rows * cols * sizeof(double));

    if (r == NULL) {
        errx(EXIT_FAILURE, "%s", MALLOC_FAILURE);
        return NULL;
    }


    for (size_t x = 0; x < cols; x++) {
        for (size_t y = 0; y < rows; y++) {
            r[y * cols + x] = m1[y * cols + x] - m2[y * cols + x];
        }
    }

    return r;
}

double *matd_mul(double *m1, double *m2, size_t r1, size_t c1, size_t c2)
{
    if (m1 == NULL || m2 == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return NULL;
    }

    double *r = malloc(r1 * c2 * sizeof(double));

    if (r == NULL) {
        errx(EXIT_FAILURE, "%s", MALLOC_FAILURE);
        return NULL;
    }

    for (size_t y = 0; y < r1; y++) {
        for (size_t x = 0; x < c2; x++) {
            r[y * c2 + x] = 0;

            for (size_t i = 0; i < c1; i++) {
                r[y * c2 + x] += m1[y * c1 + i] * m2[i * c2 + x];
            }
        }
    }

    return r;
}

double matd_get(double *mat, size_t rows, size_t cols, size_t x, size_t y) {
    if (mat == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return -1;
    }

    if (x >= cols || y >= rows) {
        errx(EXIT_FAILURE, "%s", "Index out of bound exception.");
        return -1;
    }

    return mat[y * cols + x];
}

void matd_set(double *mat, size_t rows, size_t cols, size_t x, size_t y,
        double val) {
    if (mat == NULL) {
        errx(EXIT_FAILURE, "%s", NULL_POINTER_FAILURE);
        return;
    }

    if (x >= cols || y >= rows) {
        errx(EXIT_FAILURE, "%s", "Index out of bound exception.");
        return;
    }

    mat[y * cols + x] = val;
}


void matd_free(double *mat) {
    if (mat == NULL) {
        errx(EXIT_FAILURE, "%s", "Attempting to double free pointer.");
        return;
    }

    free(mat);
}
