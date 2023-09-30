#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

double drand(double min, double max) {
    return ((float) rand() / (float) RAND_MAX) * (max - min) + min;
}

double *get_random_bits(size_t n) {
    double *r = malloc(n * sizeof(double));

    for (size_t i = 0; i < n; i++) {
        r[i] = drand(0, 1);
        if (r[i] < 0.5)
            r[i] = 0;
        else
            r[i] = 1;
    }

    return r;
}
