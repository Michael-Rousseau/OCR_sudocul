#pragma once


double squared_distance_cost(double expected, double actual);

double descent_gradient(double **network, size_t layers, double *labels, size_t len);

