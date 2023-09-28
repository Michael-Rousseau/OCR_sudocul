#include "matrix.h"
#include <stddef.h>

int main() {
    size_t ROWS = 2;
    size_t COLS = 3;

    double *mat = matrixd(ROWS, COLS);
    double *mat2 = matrixd(COLS, ROWS);

    matd_set(mat, ROWS, COLS, 2, 1, 2);

    double *r = matd_mul(mat, mat2, ROWS, COLS, ROWS);

    matd_print(mat, ROWS, COLS);
    matd_print(mat2, COLS, ROWS);
    matd_print(r, ROWS, ROWS);

    matd_free(mat);
    matd_free(mat2);
    matd_free(r);
}
