#pragma once

double drand(double min, double max);
double *get_random_bits(size_t n);
int is_int(const char *str);
int is_decimal(const char *str);
double diff_timespec(const struct timespec* t1, const struct timespec* t0);
