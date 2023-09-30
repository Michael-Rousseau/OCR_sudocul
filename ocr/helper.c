#include <stdlib.h>
#include <stdio.h>

double drand(double min, double max) {
    return ((float) rand() / (float) RAND_MAX) * (max - min) + min;
}
