#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "network.h"
#include "helper.h"

size_t LAYER_COUNT = 3;
unsigned int RANDOM_SEED = 42;

int main() {
    srand(RANDOM_SEED);

    size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
    layers[0] = 2;
    layers[1] = 2;
    layers[2] = 1;

    network *n = init_network(layers, LAYER_COUNT);
    printf("LENGTH: %zu\n", n->len);
    printf("Initialized network.\n");
    free_network(n);
    printf("Freed network.\n");

    double *i1 = get_random_bits(20);
    for (size_t i = 0; i < 20; i++) {
        printf("%1.0f ", i1[i]);
    }
    printf("\n");

    return 0;
}
