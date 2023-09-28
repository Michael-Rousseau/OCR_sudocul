#pragma once
#include <stddef.h>


double sigmoid(int x);

double* vecd_sigmoid(double* vector, size_t len);

double prime_sigmoid(int x);

double* vecd_prime_sigmoid(double* vector, size_t len);

