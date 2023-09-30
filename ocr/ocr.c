#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "helper.h"
#include "network.h"

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double prime_sigmoid(double x) {
    double sigx = sigmoid(x);
    return sigx * (1 - sigx);
}

void feed_forward(network *n, double *inputs) {

}

double read_output(network *n) {

}
