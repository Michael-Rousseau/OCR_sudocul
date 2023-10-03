#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "helper.h"
#include "network.h"
#include "ocr.h"

size_t LAYER_COUNT = 3;
unsigned int RANDOM_SEED = 42;

void test_network(network *n, size_t nb_tests) {
    double *input1 = get_random_bits(nb_tests);
    double *input2 = get_random_bits(nb_tests);

    char booleans[][6] = {"FALSE", "TRUE "};

    double valid = 0;

    n->values[0][0] = input1[0];
    n->values[0][1] = input2[0];
    for (size_t i = 0; i < nb_tests; i++) {
        double input[] = {input1[i], input2[i]};
        int expected = (((int)input1[i]) + ((int)input2[i])) % 2;

        feed_forward(n, input);
        int result;
        if (n->values[n->len - 1][0] < 0.5)
            result = 0;
        else
            result = 1;

        int bool_to_print = 0;
        if (result == expected) {
            bool_to_print = 1;
            valid++;
        }

        double *target = malloc(sizeof(double));
        *target = (double)expected;
        back_prop(n, target);
        learn(n, 0.1);

        free(target);
    }

    printf("\nRATE: %3.2f%%\n", 100 * valid / ((double)nb_tests));

    free(input1);
    free(input2);
}

int main() {
    srand(RANDOM_SEED);

    size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
    layers[0] = 2;
    layers[1] = 10;
    layers[2] = 1;

    network *n = rand_init_network(layers, LAYER_COUNT, -1, 1, -1, 1);

    test_network(n, 100000);

    free_network(n);
    return 0;
}
