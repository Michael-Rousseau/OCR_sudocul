#pragma once
#include <stddef.h>

double sigmoid(double x);

double* vecd_sigmoid(double* vector, size_t len);

double prime_sigmoid(double x);

double* vecd_prime_sigmoid(double* vector, size_t len);

