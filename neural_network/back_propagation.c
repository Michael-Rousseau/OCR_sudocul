#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "sigmoid.h"

double d_backpropagation(double **network,size_t *sizes, size_t layers, double **weight, double **biases, double learning_rate)
{
    double error_rate = 0;
    size_t output_layer = layers - 1;

    double **delta = malloc(layers * sizeof(double*));
    for (size_t i = 0; i < layers; i++) {
        delta[i] = malloc(sizes[i] * sizeof(double));
    }

    for (size_t i = 0; i < sizes[output_layer]; ++i) {
        double target = 1;
        double output = network[output_layer][i];
        delta[output_layer][i] = (output - target) * sigmoid(output);
        error_rate += 0.5 * ((output - target)* (output-target));
    }

    for (size_t l = output_layer; l > 0; --l) {
        for (size_t i = 0; i < sizes[l]; ++i) {
            delta[l][i] = sigmoid(network[l][i]) * delta[l][i];

            for (size_t j = 0; j < sizes[l - 1]; ++j) {
                double delta_weight = learning_rate * delta[l][i] * network[l - 1][j];
                weight[l - 1][i * sizes[l - 1] + j] -= delta_weight;
            }

            double delta_bias = learning_rate * delta[l][i];
            biases[l - 1][i] -= delta_bias;
        }

        if (l > 1) {
            for (size_t j = 0; j < sizes[l - 1]; ++j) {
                double sum = 0;
                for (size_t k = 0; k < sizes[l]; ++k) {
                    sum += weight[l - 1][k * sizes[l - 1] + j] * delta[l][k];
                }
                delta[l - 1][j] = sum;
            }
        }
    }

    for (size_t i = 0; i < layers; i++) {
        free(delta[i]);
    }
    free(delta);

    return error_rate;
}
