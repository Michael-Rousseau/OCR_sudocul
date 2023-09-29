#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "sigmoid.h"

double **init_network(size_t *sizes, size_t layers) {
    double **network = malloc(layers * sizeof(double*));

    for (size_t i = 0; i < layers; i++) {
        network[i] = malloc(sizes[i] * sizeof(double));

        for (size_t j = 0; j < sizes[i]; j++) {
            network[i][j] = 0;
        }
    }

    return network;
}

void set_input_layer(double **network, double *values, size_t len) {
    for (size_t i = 0; i < len; i++) {
        network[0][i] = values[i];
    }
}

void feed_forward(double **network, size_t *sizes, size_t layer, 
        double **weight, double **biases)
{
    size_t len_i = sizes[layer - 1];
    size_t len_o = sizes[layer];

    for (size_t i = 0; i < len_o; i++) {
        double sum = 0;

        for (size_t j = 0; j < len_i; j++) {
            sum += network[layer - 1][j] * weight[layer - 1][len_i * i + j];
        }

        sum += biases[layer - 1][i];
        network[layer][i] = sigmoid(sum);
    }
}

double read_output(double **network, size_t *sizes, size_t layers) {
    size_t len_o = sizes[layers - 1];

    size_t max = 0;

    for (size_t i = 0; i < len_o; i++) {
        double val = network[layers - 1][i];

        if (val > network[layers - 1][max]) {
            max = i;
        }
    }

    return network[layers - 1][max];
}

void free_network(double **network, size_t layers) {
    for (size_t i = 0; i < layers; i++) {
        free(network[i]);
    }

    free(network);
}

