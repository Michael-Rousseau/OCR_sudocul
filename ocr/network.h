#pragma once

#include <stddef.h>

struct struct_network {
    double **values;

    size_t len;
    size_t *layers;

    double **biases;
    double ***weights;
};

typedef struct struct_network network;

network *init_network(size_t *layers, size_t len);
network *rand_init_network(size_t *layers, size_t len,
        double wmin, double wmax, double bmin, double bmax);

void import_network(network *n, double ***w, double **b);
void export_network(network *n);

network *free_network(network *network);
