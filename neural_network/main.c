#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include <time.h>
#include "back_propagation.h"
#include "cost.h"

void test(double **network, size_t *sizes, double **weights, double **biases)
{
    double *input = malloc(2 * sizeof(double));
    input[0] = 0;
    input[1] = 0;

    double expected = input[0] + input[1];
    if (expected > 1) expected = 0;

    set_input_layer(network, input, 2);
    feed_forward(network, sizes, 1, weights, biases);
    feed_forward(network, sizes, 2, weights, biases);

    double result = read_output(network, sizes, 3);
    printf("TEST x1=%.1f x2=%.1f\n", input[0], input[1]);
    printf("\tExpected: %.1f\n", expected);
    printf("\tActual  : %.1f\n", result);

    input[0] = 1;
    input[1] = 0;

    expected = input[0] + input[1];
    if (expected > 1) expected = 0;

    set_input_layer(network, input, 2);
    feed_forward(network, sizes, 1, weights, biases);
    feed_forward(network, sizes, 2, weights, biases);

    result = read_output(network, sizes, 3);
    printf("\nTEST x1=%.1f x2=%.1f\n", input[0], input[1]);
    printf("\tExpected: %.1f\n", expected);
    printf("\tActual  : %.1f\n", result);

    input[0] = 1;
    input[1] = 1;

    expected = input[0] + input[1];
    if (expected > 1) expected = 0;

    set_input_layer(network, input, 2);
    feed_forward(network, sizes, 1, weights, biases);
    feed_forward(network, sizes, 2, weights, biases);

    result = read_output(network, sizes, 3);
    printf("\nTEST x1=%.1f x2=%.1f\n", input[0], input[1]);
    printf("\tExpected: %.1f\n", expected);
    printf("\tActual  : %.1f\n", result);

    input[0] = 0;
    input[1] = 1;

    expected = input[0] + input[1];
    if (expected > 1) expected = 0;

    set_input_layer(network, input, 2);
    feed_forward(network, sizes, 1, weights, biases);
    feed_forward(network, sizes, 2, weights, biases);

    result = read_output(network, sizes, 3);
    printf("\nTEST x1=%.1f x2=%.1f\n", input[0], input[1]);
    printf("\tExpected: %.1f\n", expected);
    printf("\tActual  : %.1f\n", result);

    free(input);
}

void randomize_weights(double *weights, size_t size, double min, double max)
{
    for (size_t i = 0; i < size; i++)
    {
        weights[i] = (rand() / (double)RAND_MAX) * (max - min) + min;
    }
}

int main() {
    // Initialize random seed for weight and bias initialization
    srand(time(NULL));

    size_t layers = 3;
    size_t sizes[] = { 2, 2, 1 };
    double w0[4];  // Array to store random weights for layer 0
    double w1[2];  // Array to store random weights for layer 1

    // Initialize weights with random values between -1 and 1
    randomize_weights(w0, 4, -1.0, 1.0);
    randomize_weights(w1, 2, -1.0, 1.0);

    double **weights = malloc(2 * sizeof(double*));
    weights[0] = w0;
    weights[1] = w1;

    // Initialize biases with random values between -1 and 1
    double b0[] = { (rand() / (double)RAND_MAX) * 2 - 1, (rand() / (double)RAND_MAX) * 2 - 1 };
    double b1[] = { (rand() / (double)RAND_MAX) * 2 - 1 };
    double **biases = malloc(2 * sizeof(double*));
    biases[0] = b0;
    biases[1] = b1;

    double **network = init_network(sizes, 3);

    // Training loop with backpropagation
    int epochs = 10000;
    double learning_rate = 0.1;

    for (int epoch = 0; epoch < epochs; epoch++) {
        double training_data[][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };

        for (int i = 0; i < 4; i++) {
            double *input = training_data[i];
            double expected = input[0] + input[1];
            if (expected > 1) expected = 0;

            set_input_layer(network, input, 2);

            for (size_t j = 1; j < layers; j++)
                feed_forward(network, sizes, j, weights, biases);

            // Backpropagate and update weights and biases
            d_back_propagation(network, sizes, 3, weights, biases, learning_rate, &expected);
        }
    }

    test(network, sizes, weights, biases);

    for (size_t i = 0; i < layers - 1; i++) {
        free(weights[i]);
        free(weights[i]);
    }

    free(weights);
    free(biases);
    free_network(network, layers);

    return 0;
}
