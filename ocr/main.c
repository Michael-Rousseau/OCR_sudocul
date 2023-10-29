#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "../fileStream/files.h"
#include "helper.h"
#include "network.h"
#include "ocr.h"

unsigned int RANDOM_SEED = 42;

void test_xor_network(network *n, size_t nb_tests) {
    double *input1 = get_random_bits(nb_tests);
    double *input2 = get_random_bits(nb_tests);

    double valid = 0;

    double *input = malloc(2 * sizeof(double));
    double *target = malloc(sizeof(double));

    double j = 0;
    for (size_t i = 0; i < nb_tests; i++) {
        double i1 = input1[i];
        double i2 = input2[i];
        input[0] = i1;
        input[1] = i2;

        int expected = ((int)i1 + (int)i2) % 2;

        feed_forward(n, input);
        int result;
        if (n->values[n->len - 1][0] < 0.5)
            result = 0;
        else
            result = 1;

        if (result == expected)
            valid++;

        *target = (double)expected;
        back_prop(n, target);
        learn(n, 0.9);

        if (j == 1000) {
            if (100 * valid / j >= 100) {
                printf("Finished learning in %zu tries\n", i);
                break;
            }
            printf("RATE: %3.2f%% (%zu)\n", 100 * valid / j, i);

            j = 1;
            valid = 0;
        }
        else
            j++;
    }

    free(input);
    free(target);
    free(input1);
    free(input2);
}

void usage_error() {
    printf("Usage:\n");
    printf("TEST : ./main [-f file]|[-lh N N...] test NBTEST\n");
    printf("LEARN: ./main [-f file]|[-lh N N...] learn NBTEST RATE\n");
    errx(EXIT_FAILURE, "%s\n", "Wrong usage.");
}

int main(int argc, char **argv) {
    srand(RANDOM_SEED);

    if (argc < 3)
        usage_error();

    int i = 1;

    size_t layer_count = 2;
    size_t *layers = malloc(2 * sizeof(double));
    network *n;

    int mode = -1;
    size_t nb_tests = 0;
    double learning_rate = 0;

    while (i < argc) {
        if (strcmp("-f", argv[i]) == 0) {
            i += 1;
            if (i < argc)
                n = import_network(argv[i]);
            else
                usage_error();
            i++;
        }

        else if (strcmp("-lh", argv[i]) == 0) {
            i++;

            if (!is_int(argv[i]))
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

    printf("DONE!");

    free_network(n);
    return 0;
}
