#include <stdio.h>
#include <stdlib.h>

double **matrixd(int rows, int cols) {
    double** mat = (double**) malloc(rows * sizeof(double*));
    if (mat == NULL) {
        perror("Memory allocation failed.");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        mat[i] = (double*) malloc(cols * sizeof(double));
        if (mat[i] == NULL) {
            perror("Memory allocation failed.");
            return NULL;
        }
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            mat[y][x] = (double) 0;
        }
    }

    return mat;
}

double **matrix2d(double a, double b, double c, double d) {
    double **mat = matrixd(2, 2);
    mat[0][0] = a;
    mat[0][1] = b;
    mat[1][0] = c;
    mat[1][1] = d;

    return mat;
}
