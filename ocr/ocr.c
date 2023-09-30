#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "helper.h"
#include "network.h"

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double prime_sigmoid(double x) {
    double sigx = sigmoid(x);
    return sigx * (1 - sigx);
}

void feed_forward(network *n, double *inputs) {
    for (size_t layer = 1; layer < n->len; layer++) {
        for (size_t i = 0; i < n->layers[layer]; i++) {
            double sum = 0;

            for (size_t j = 0; j < n->layers[layer - 1]; j++)
                sum += n->values[i - 1][j];

            n->values[layer][i] = sigmoid(sum + n->biases[layer - 1][i]);
        }
    }
}

size_t read_output(network *n) {
    size_t last = n->len - 1;

    size_t max = 0;
    for (size_t i = 1; i < n->layers[last]; i++) {
        if (n->values[last][i] > n->values[last][max])
            max = i;
    }

    return max;
}
