#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    network_to_graph(n, "results/test_before");
    for (size_t i = 0; i < 2; i++) {
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
        back_propagation(n, 0.1, target);

        if (1) {
            char *graph_path;
            asprintf(&graph_path, "results/learning_results%zu", i);
            network_to_graph(n, graph_path);
            free(graph_path);
        }
        free(target);

        printf("TEST n%5zu: %s (%1.0f XOR %1.0F = %hhi EXPECTED %hhi)\n", i + 1,
               booleans[bool_to_print], input1[i], input2[i], result, expected);
    }

    network_to_graph(n, "results/test_after");
    printf("\nRATE: %3.2f%%\n", 100 * valid / ((double)nb_tests));

    free(input1);
    free(input2);
}

int main() {
    srand(RANDOM_SEED);

    size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
    layers[0] = 2;
    layers[1] = 2;
    layers[2] = 1;

    network *n = rand_init_network(layers, LAYER_COUNT, -1, 1, -1, 1);

    test_network(n, 100000);

    free_network(n);
    return 0;
}
