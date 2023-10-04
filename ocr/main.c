#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fileStream/files.h"
#include "helper.h"
#include "network.h"
#include "ocr.h"

size_t LAYER_COUNT = 3;
unsigned int RANDOM_SEED = 42;

void test_network(network *n, size_t nb_tests) {
    double *input1 = get_random_bits(nb_tests);
    double *input2 = get_random_bits(nb_tests);

    double valid = 0;

    double *input = malloc(2 * sizeof(double));
    double *target = malloc(sizeof(double));

    double j = 0;
    for (size_t i = 0; i < nb_tests; i++) {
        double i1 = input1[i];
        double i2 = input2[i];
        input[0] = i1;
        input[1] = i2;

        int expected = ((int)i1 + (int)i2) % 2;

        feed_forward(n, input);
        int result;
        if (n->values[n->len - 1][0] < 0.5)
            result = 0;
        else
            result = 1;

        if (result == expected)
            valid++;

        *target = (double)expected;
        back_prop(n, target);
        learn(n, 0.9);

        if (j == 1000) {
            if (100 * valid / j >= 100) {
                printf("Finished learning in %zu tries\n", i);
                break;
            }
            printf("RATE: %3.2f%% (%zu)\n", 100 * valid / j, i);

            j = 1;
            valid = 0;
        }
        else
            j++;
    }

    free(input);
    free(target);
    free(input1);
    free(input2);
}

int main() {
    srand(RANDOM_SEED);

    size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
    layers[0] = 2;
    layers[1] = 4;
    layers[2] = 1;

    network *n = rand_init_network(layers, LAYER_COUNT, -1, 1, -1, 1);

    //network *n = import_network("xor.trained");

    test_network(n, 5000000);
    export_network(n, "xor.trained");

    free_network(n);
    return 0;
}
