#pragma once
#include "network.h"
#include <bits/pthreadtypes.h>
void feed_forward(network *n, double *inputs);
void back_prop(network *n, int *expected);
void learn(network *n, double speed);
size_t read_output(network *n);

struct ThreadData {
    network *n;
    size_t start;
    size_t end;
    pthread_mutex_t *mutex;
    int* expected;
    double speed;
};
