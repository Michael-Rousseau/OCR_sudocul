#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "helper.h"
#include "network.h"
#include "ocr.h"

unsigned int RANDOM_SEED = 42;

int test_network(network *n, double *input, double *target) {
    int i1 = (int) floor(drand(0, 2));
    int i2 = (int) floor(drand(0, 2));

    input[0] = (double) i1;
    input[1] = (double) i2;
    target[0] = (double) (i1 ^ i2);

    feed_forward(n, input);
    back_prop(n, target);

    return floor(n->values[n->len - 1][0] * 2) == *target;
}

void usage_error() {
    FILE *file_in;
    file_in = fopen("data/man_xor", "r");

    if (file_in == NULL)
        errx(EXIT_FAILURE, "%s\n", "Usage error, and man page wasn't found.");

    size_t i = 0;
    char *line = calloc(1000, sizeof(char));
    while(fgets(line, 1000, file_in)) {
        i++;
        printf("%s", line);

        for (size_t i = 0; i < 1000; i++)
            line[i] = 0;
    }

    free(line);
    fclose(file_in);

    errx(EXIT_FAILURE, "%s\n", "Wrong usage.");
}
/*
int main(int argc, char **argv) {
    // srand(RANDOM_SEED);

    if (argc < 3)
        usage_error();

    int i = 1;

    size_t layer_count = 2;
    size_t *layers = malloc(2 * sizeof(double));
    network *n;

    int mode = -1;
    size_t nb_tests = 0;
    double learning_rate = 0;

    char *save_path = NULL;

    while (i < argc) {
        if (strcmp("-f", argv[i]) == 0) {
            i++;
            free(layers);

            if (i < argc)
                n = import_network(argv[i]);
            else
                usage_error();
            i++;
        }

        else if (strcmp("-lh", argv[i]) == 0) {
            i++;

            if (i >= argc || !is_int(argv[i]))
                usage_error();

            layers[0] = 2;
            while (is_int(argv[i])) {
                layers[layer_count - 1] = (size_t) strtol(argv[i], NULL, 10);
                layer_count++;
                layers = realloc(layers, layer_count * sizeof(double));
                i++;
            }
            layers[layer_count - 1] = 1;
            n = rand_init_network(layers, layer_count, -1, 1, -1, 1);
        }
        else if (strcmp("-s", argv[i]) == 0) {
            i++;

            if (i >= argc)
                save_path = "network.nw";
            else
                save_path = argv[i];

            i++;
        }
        else if (strcmp("test", argv[i]) == 0) {
            mode = 1;
            i++;

            if (i >= argc || !is_int(argv[i]))
                usage_error();

            nb_tests = (size_t) strtol(argv[i], NULL, 10);
            i++;
        }
        else if (strcmp("learn", argv[i]) == 0) {
            mode = 0;
            i++;

            if (i + 1 >= argc
                    || !is_int(argv[i])
                    || !is_decimal(argv[i + 1]))
            {
                usage_error();
            }

            nb_tests = (size_t) strtol(argv[i], NULL, 10);
            i++;
            learning_rate = strtod(argv[i], NULL);
            i++;
        }
        else
            usage_error();
    }

    if (NULL == n)
        usage_error();

    struct timespec t0;
    clock_gettime(0, &t0);

    if (1 == mode) {
        size_t valid = 0;
        double *input = malloc(2 * sizeof(double));
        double *output = malloc(sizeof(double));

        if (NULL == input || NULL == output)
            errx(EXIT_FAILURE, "%s\n", "Memory allocation failed");

        for (size_t i = 0; i < nb_tests; i++)
            valid += test_network(n, input, output);

        struct timespec t1;
        clock_gettime(0, &t1);
        printf("TEST RESULTS: %zu/%zu (%fs)\n", valid, nb_tests,
                diff_timespec(&t1, &t0));

        if (NULL != save_path) {
            export_network(n, save_path);
            struct timespec t2;
            clock_gettime(0, &t2);
            printf("NETWORK EXPORTED IN %fs\n", diff_timespec(&t2, &t1));
        }

        free(input);
        free(output);
    }
    else if (0 == mode) {
        size_t valid = 0;
        double *input = malloc(2 * sizeof(double));
        double *output = malloc(sizeof(double));

        if (NULL == input || NULL == output)
            errx(EXIT_FAILURE, "%s\n", "Memory allocation failed");

        for (size_t i = 0; i < nb_tests; i++) {
            valid += test_network(n, input, output);
            learn(n, learning_rate);
        }

        struct timespec t1;
        clock_gettime(0, &t1);
        printf("LEARN RESULTS: %zu TESTS IN %fs\n", nb_tests,
                diff_timespec(&t1, &t0));

        if (save_path) {
            export_network(n, save_path);
            struct timespec t2;
            clock_gettime(0, &t2);
            printf("NETWORK EXPORTED IN %fs\n", diff_timespec(&t2, &t1));
        }

        free(input);
        free(output);
    }
    else
        usage_error();

    free_network(n);
    return 0;
}
*/
