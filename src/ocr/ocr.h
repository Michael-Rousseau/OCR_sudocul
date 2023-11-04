#pragma once

void feed_forward(network *n, double *inputs);
void back_propagation(network *net, double learning_rate, double* target);
void back_prop(network *n, double *expected);
void learn(network *n, double speed);
size_t read_output(network *n);
