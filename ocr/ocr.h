#pragma once

void feed_forward(network *n, double *inputs);
void back_propagation(network *net, double learning_rate, double* target);
size_t read_output(network *n);
