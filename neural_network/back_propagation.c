#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "sigmoid.h"


void d_back_propagation(double **network,size_t *sizes, size_t layers, double **weight, double **biases, double learning_rate, double* target)
{
    *weight = malloc((layers - 1) * sizeof(double *));
    *biases = malloc((layers - 1) * sizeof(double *));

    for (size_t i = 0; i < layers - 1; i++) {
        size_t len_i = sizes[i];
        size_t len_o = sizes[i + 1];

        (weight)[i] = malloc(len_i * len_o * sizeof(double));
        (biases)[i] = malloc(len_o * sizeof(double));

        for (size_t j = 0; j < len_i * len_o; j++) {
            (weight)[i][j] = 0.0;
        }

        for (size_t j = 0; j < len_o; j++) {
            (biases)[i][j] = 0.0;
        }
    }

       for (size_t layer = layers - 1; layer >= 1; layer--) {
        size_t len_i = sizes[layer - 1];
        size_t len_o = sizes[layer];

        for (size_t i = 0; i < len_o; i++) {
            double output = network[layer][i];
            double delta = 0.0;

            if (layer == layers - 1) {
                delta = (output - target[i]) * (output);
            } else {
                for (size_t j = 0; j < sizes[layer + 1]; j++) {
                    delta += weight[layer][len_i * j + i] * network[layer + 1][j];
                }
                delta *= prime_sigmoid(output);  // Corrected here
            }

            biases[layer - 1][i] += delta;
            for (size_t j = 0; j < len_i; j++) {
                weight[layer - 1][len_i * i + j] += delta * network[layer - 1][j];
            }
        }
    }

    for (size_t layer = 0; layer < layers - 1; layer++) {
        size_t len_i = sizes[layer];
        size_t len_o = sizes[layer + 1];

        for (size_t i = 0; i < len_o; i++) {
            biases[layer][i] -= learning_rate * biases[layer][i];  // Corrected here
            for (size_t j = 0; j < len_i; j++) {
                weight[layer][len_i * i + j] -= learning_rate * weight[layer][len_i * i + j];  // Corrected here
            }
        }
    }
}
