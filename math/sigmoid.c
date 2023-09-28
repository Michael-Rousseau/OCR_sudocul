#include <stdio.h>
#include <math.h>

double sigmoid(int x)
{
    return 1/(1+exp(-x));
}

double* vecd_sigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        int tmp = *(vector +i);
        *(vector+i) = calcSigmoid(tmp);
    }
    return vector;
}

double prime_sigmoid(int x)
{
    double sigx = calcSigmoid(x);
    return sigx * (1- sigx);
}


double* vecd_prime_sigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        int tmp = *(vector +i);
        *(vector+i) = calcDerivateSigmoid(tmp);
    }
    return vector;
}
