#include <stdio.h>
#include <math.h>
#include "sigmoid.h"

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double* vecd_sigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        double tmp = *(vector +i);
        *(vector+i) = sigmoid(tmp);
    }
    return vector;
}

double prime_sigmoid(double x)
{
    double sigx = sigmoid(x);
    return sigx * (1 - sigx);
}


double* vecd_prime_sigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        double tmp = *(vector +i);
        *(vector+i) = prime_sigmoid(tmp);
    }

    return vector;
}
