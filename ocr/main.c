#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "helper.h"
#include "network.h"
#include "ocr.h"

unsigned int RANDOM_SEED = 42;

void gen_xor_input(double **input, double **expected) {
    *input = get_random_bits(2);
    *expected = malloc(sizeof(double));
    **expected = (int) **input ^ (int) input[0][1];
}

int test_network(network *n) {
    double *input;
    double *target;

    gen_xor_input(&input, &target);
    feed_forward(n, input);

    back_prop(n, target);

    double result = floor(n->values[n->len - 1][0] * 2) == *target;

    free(target);
    free(input);
    return result;
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

    char *save_path;

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

    if (1 == mode) {
        size_t valid = 0;
        for (size_t i = 0; i < nb_tests; i++)
            valid += test_network(n);

        printf("%zu/%zu\n", valid, nb_tests);
        if (save_path)
            export_network(n, save_path);
    }
    else if (0 == mode) {
        size_t valid = 0;
        for (size_t i = 0; i < nb_tests; i++) {
            valid += test_network(n);
            learn(n, learning_rate);
        }

        printf("%zu/%zu\n", valid, nb_tests);
        if (save_path)
            export_network(n, save_path);
    }
    else
        usage_error();


    free_network(n);
    return 0;
}
