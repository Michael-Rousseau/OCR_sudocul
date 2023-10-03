#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "helper.h"
#include "network.h"

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double prime_sigmoid(double x) {
    double sigx = (x);
    return sigx * (1 - sigx);
}

void feed_forward(network *n, double *inputs) {
    for (size_t i = 0; i < n->layers[0]; i++)
        n->values[0][i] = inputs[i];

    for (size_t layer = 1; layer < n->len; layer++) {
        for (size_t i = 0; i < n->layers[layer]; i++) {

            double sum = 0;

            for (size_t j = 0; j < n->layers[layer - 1]; j++) {
                sum += n->values[layer - 1][j] * n->weights[layer - 1][i][j];
            }

            n->values[layer][i] = sigmoid(sum + n->biases[layer - 1][i]);
        }
    }
}

void v_feed_forward(network *n, double *inputs) {
    for (size_t i = 0; i < n->layers[0]; i++)
        n->values[0][i] = inputs[i];

    for (size_t layer = 1; layer < n->len; layer++) {
        printf("LAYER %zu\n", layer);
        for (size_t i = 0; i < n->layers[layer]; i++) {
            printf("NODE %zu\n", i + 1);

            double sum = 0;

            for (size_t j = 0; j < n->layers[layer - 1]; j++) {
                sum += n->values[layer - 1][j] * n->weights[layer - 1][i][j];
                printf("sum += %2.2f * %2.2f (=%2.2f)\n",
                        n->values[layer - 1][j],
                        n->weights[layer - 1][i][j],
                        sum);
            }

            n->values[layer][i] = sigmoid(sum + n->biases[layer - 1][i]);
            printf("New node value: %2.2f\n", n->values[layer][i]);
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

void back_prop(network *n, double *expected) {
    size_t last = n->len - 1;

    for (size_t i = 0; i < n->layers[last]; i++) {
        double v = n->values[last][i];
        n->costs[last - 1][i] = v * (1 - v) * (v - expected[i]);
    }

    for (size_t i = last - 1; i > 0; i--) {
        for (size_t j = 0; j < n->layers[i + 1]; j++) {
            n->costs[i - 1][j] = 0;

            for (size_t k = 0; k < n->layers[i]; k++) {
                n->costs[i - 1][j] += n->weights[i][j][k] * n->costs[i][j];
            }

            n->costs[i - 1][j] *= prime_sigmoid(n->values[i][j]);
        }
    }
}

void learn(network *n, double speed) {
    for (size_t i = 0; i < n->len - 1; i++) {
        for (size_t j = 0; j < n->layers[i]; j++)
        {
            for(size_t k = 0; k < n->layers[i + 1]; k++)
            {
                n->weights[i][k][j] -= speed *
                    n->values[i][j] *
                    n->costs[i][k];
            }
        }

        for (size_t j = 0; j < n->layers[i + 1]; j++) {
            n->biases[i][j] -= speed * n->costs[i][j];
        }
    }
}

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
            double delta = 0;

            for (size_t j = 0; j < sizes[layer + 1]; j++) {
                delta += weights[layer][j][i] * prime_sigmoid(output);
            }

            biases[layer - 1][i] += delta * learning_rate;

            for (size_t j = 0; j < sizes[layer - 1]; j++) {
                weights[layer - 1][i][j] += delta * network_values[layer - 1][j] * learning_rate;
            }
        }
    }
}

