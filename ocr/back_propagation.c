#include <stdio.h>
#include <stdlib.h>
#include "ocr.h"

void back_propagation(network *net, double learning_rate, double* target)
{
    size_t layers = net->len;
    size_t *sizes = net->layers;
    double **network_values = net->values;
    double ***weights = net->weights;
    double **biases = net->biases;

    size_t output_layer = layers - 1;
    for (size_t i = 0; i < sizes[output_layer]; i++) {
        double output = network_values[output_layer][i];
        double delta = (output - target[i]) * prime_sigmoid(output);
        biases[output_layer - 1][i] += delta * learning_rate;

        for (size_t j = 0; j < sizes[output_layer - 1]; j++) {
            weights[output_layer - 1][i][j] += delta * network_values[output_layer - 1][j] * learning_rate;
        }
    }

    for (size_t layer = output_layer - 1; layer > 0; layer--) {
        for (size_t i = 0; i < sizes[layer]; i++) {
            double output = network_values[layer][i];
            double delta = 0.0;

            for (size_t j = 0; j < sizes[layer + 1]; j++) {
                delta += weights[layer][j][i] * prime_sigmoid(output) * biases[layer][j];
            }

            biases[layer - 1][i] += delta * learning_rate;

            for (size_t j = 0; j < sizes[layer - 1]; j++) {
                weights[layer - 1][i][j] += delta * network_values[layer - 1][j] * learning_rate;
            }
        }
    }
}

