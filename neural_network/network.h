#pragma once


double **init_network(size_t *sizes, size_t layers);

void set_input_layer(double **network, double *values, size_t len);

void feed_forward(double **network, size_t *sizes, size_t layer, double **weight, double **biases);

double read_output(double **network, size_t *sizes, size_t layers);

void free_network(double **network, size_t layers);

