#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"

void test(double **network, size_t *sizes, double **weights, double **biases) {
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

int main() {
    size_t sizes[] = { 2, 2, 1 };
    double w0[] = { 20, 20, -20, -20 };
    double w1[] = { 20, 20 };

    double **weights = malloc(2 * sizeof (double*));
    weights[0] = w0;
    weights[1] = w1;

    double b0[] = { -10, 30 };
    double b1[] = { -30 };
    double **biases = malloc(2 * sizeof(double*));
    biases[0] = b0;
    biases[1] = b1;

    double **network = init_network(sizes, 3);
    test(network, sizes, weights, biases);

    free_network(network, 3);
    free(weights);
    free(biases);

    return 0;
}
