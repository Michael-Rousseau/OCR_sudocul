#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "network.h"
#include "ocr.h"

size_t LAYER_COUNT = 3;
unsigned int RANDOM_SEED = 42;

void create_graph(network *n, char *path) {
    FILE *file_in;
    file_in = fopen("xor_graph_format.dot", "r");

    char *text = malloc(sizeof(char));
    *text = 0;

    char line[100];
    while (fgets(line, 100, file_in)) {
        text = realloc(text, strlen(text) + strlen(line) + 1);
        strcat(text, line);
    }
    fclose(file_in);

    char *output;
    double inputs[] = {
    };

    double **biases = n->biases;
    double ***weights = n->weights;
    asprintf(&output, text, n->values[0][0], n->values[0][1],
            biases[0][0], biases[0][1], biases[1][0],
            weights[0][0][0], weights[0][0][1],
            weights[0][1][0], weights[0][1][1],
            weights[1][0][0], weights[1][0][1]);
    free(text);

    FILE *file_out;
    char *dest;
    asprintf(&dest, "%s.dot", path);
    file_out = fopen(dest, "w");
    fprintf(file_out, output);
    fclose(file_out);

    char *new_dest;
    asprintf(&new_dest, "dot -Tpdf %s.dot > %s.pdf", path, path);
    printf("%s\n", new_dest);
    system(new_dest);

    free(output);
    free(dest);
    free(new_dest);
}

void test_network(network *n, size_t nb_tests) {
    double *input1 = get_random_bits(nb_tests);
    double *input2 = get_random_bits(nb_tests);

    char booleans[][6] = {"FALSE", "TRUE "};

    double valid = 0;

    for (size_t i = 0; i < nb_tests; i++) {
        double input[] = {input1[i], input2[i]};
        int expected = (((int)input1[i]) + ((int)input2[i])) % 2;

        feed_forward(n, input);
        int result;
        if (n->values[n->len - 1][0] < 0.5)
            result = 0;
        else
            result = 1;

        int bool_to_print = 0;
        if (result == expected) {
            bool_to_print = 1;
            valid++;
        }

        double *target = malloc(sizeof(double));
        *target = (double)expected;
        back_prop(n, target);
        free(target);

        printf("TEST n%5zu: %s (%1.0f XOR %1.0F = %hhi EXPECTED %hhi)\n", i + 1,
               booleans[bool_to_print], input1[i], input2[i], result, expected);
    }

    printf("\nRATE: %3.2f%%\n", 100 * valid / ((double)nb_tests));

    free(input1);
    free(input2);
}

int main() {
    srand(RANDOM_SEED);

    size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
    layers[0] = 2;
    layers[1] = 2;
    layers[2] = 1;

    network *n = rand_init_network(layers, LAYER_COUNT, -1, 1, -1, 1);

    // test_network(n, 1000);
    create_graph(n, "test");

    free_network(n);
    return 0;
}
