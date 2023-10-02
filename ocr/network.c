#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "network.h"
#include "helper.h"

//init the eural_network, defined by a struct containing
//a vector for values
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
            weights[i][j] = malloc(layers[i] * sizeof(double));

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

void network_to_graph(network *n, char *path) {
    FILE *file_in;
    file_in = fopen("xor_graph_format.dot", "r");

    char *text = malloc(sizeof(char));
    *text = 0;

    char line[100];
    while (fgets(line, 100, file_in)) {
        text = realloc(text, strlen(text) + strlen(line) + 1);
        strcat(text, line);
    }
    fclose(file_in);

    char *output;
    double inputs[] = {
    };

    double **biases = n->biases;
    double ***weights = n->weights;
    asprintf(&output, text,
            n->values[0][0], n->values[0][1],
            biases[0][0], n->values[1][0],
            biases[0][1], n->values[1][1],
            biases[1][0], n->values[2][0],
            weights[0][0][0], weights[0][0][1],
            weights[0][1][0], weights[0][1][1],
            weights[1][0][0], weights[1][0][1]);

    free(text);

    FILE *file_out;
    char *dest;
    asprintf(&dest, "%s.dot", path);
    file_out = fopen(dest, "w");
    fprintf(file_out, output);
    fclose(file_out);

    char *new_dest;
    asprintf(&new_dest, "dot -Tpdf %s.dot > %s.pdf", path, path);
    printf("%s\n", new_dest);
    system(new_dest);

    free(output);
    free(dest);
    free(new_dest);
}

void print_network(network *n) {
    for (size_t i = 0; i < n->len - 1; i++) {
        printf("LAYER %zu\n", i + 1);

        for (size_t j = 0; j < n->layers[i + 1]; j++) {
            printf("NODE %zu -> BIUS = %2.2f\n", j + 1, n->biases[i][j]);
            for (size_t k = 0; k < n->layers[i]; k++) {
                printf("w%zu=%2.2f ", k + 1, n->weights[i][j][k]);
            }
            printf("\n");
        }

        printf("\n");
    }
}
