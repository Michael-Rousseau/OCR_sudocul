#include <math.h>
#include <stddef.h>
#include <stdlib.h>

double squared_distance_cost(double expected, double actual) {
    double diff = actual - expected;
    return diff * diff;
}

double descent_gradient(double **network, size_t layers, double *labels, size_t len) {
    double sum = 0;

    for (size_t i = 0; i < len; i++) {
        double expected = labels[i];
        double actual = network[layers - 1][i];

        sum += squared_distance_cost(expected, actual);
    }

    return sum;
}
