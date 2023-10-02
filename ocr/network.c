#include <stdlib.h>
#include <stddef.h>

#include "network.h"
#include "helper.h"

network *init_network(size_t *layers, size_t len) {
    network *n = malloc(sizeof(network));

    double **values = malloc(len * sizeof(double*));

    for (size_t i = 0; i < len; i++) {
        values[i] = malloc(layers[i] * sizeof(double));

        for (size_t j = 0; j < layers[i]; j++) {
            values[i][j] = 0;
        }
    }

    double **biases = malloc((len - 1) * sizeof(double*));
    double **costs = malloc((len - 1) * sizeof(double*));
    double ***weights = malloc((len - 1) * sizeof(double*));

    for (size_t i = 0; i < len - 1; i++) {
        biases[i] = malloc(layers[i + 1] * sizeof(double));
        costs[i] = malloc(layers[i + 1] * sizeof(double));
        weights[i] = malloc(layers[i + 1] * sizeof(double));

        for (size_t j = 0; j < layers[i + 1]; j++) {
            costs[i][j] = 0;
            biases[i][j] = 0;
            weights[i][j] = malloc(layers[i] * sizeof(j));

            for (size_t k = 0; k < layers[i]; k++) {
                weights[i][j][k] = 0;
            }
        }
    }

    n->values = values;
    n->len = len;
    n->layers = layers;

    n->costs = costs;
    n->biases = biases;
    n->weights = weights;

    return n;
}

network *rand_init_network(size_t *layers, size_t len,
        double wmin, double wmax, double bmin, double bmax)
{
    network *n = init_network(layers, len);

    for (size_t i = 0; i < n->len - 1; i++) {
        for (size_t j = 0; j < n->layers[i + 1]; j++) {
            n->biases[i][j] = drand(bmin, bmax);
            for (size_t k = 0; k < n->layers[i]; k++) {
                n->weights[i][j][k] = drand(wmin, wmax);
            }
        }
    }

    return n;
}

network *import_network(char *path) {
    return NULL;
}

void export_network(network *n, char *path) {

}

void free_network(network *n) {
    for (size_t i = 0; i < n->len; i++)
        free((n->values)[i]);
    free(n->values);

    for (size_t i = 0; i < n->len - 1; i++) {
        for (size_t j = 0; j < n->layers[i + 1]; j++)
            free(n->weights[i][j]);

        free(n->costs[i]);
        free(n->biases[i]);
        free(n->weights[i]);
    }
    free(n->costs);
    free(n->biases);
    free(n->weights);

    free(n->layers);
    free(n);
}
