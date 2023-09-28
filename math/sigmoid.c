#include <stdio.h>
#include <math.h>

double calcSigmoid(int x)
{
    return 1/(1+exp(-x));
}

double* sigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        int tmp = *(vector +i);
        *(vector+i) = calcSigmoid(tmp);
    }
    return vector;
}

double calcDerivateSigmoid(int x)
{
    double sigx = calcSigmoid(x);
    return sigx * (1- sigx);
}


double* DerSigmoid(double* vector, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        int tmp = *(vector +i);
        *(vector+i) = calcDerivateSigmoid(tmp);
    }
    return vector;
}
