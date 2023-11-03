#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include "files.h"


char **file_readlines(char *path) {
    FILE *file_in;
    file_in = fopen(path, "r");

    if (file_in == NULL) {
        errx(EXIT_FAILURE, "%s", "File Not Found Error.");
        return NULL;
    }

    char **result = calloc(1, sizeof(char*));

    size_t i = 0;
    char *line = calloc(1000, sizeof(char));
    if(line == NULL)
        errx(EXIT_FAILURE, "failed to open file");
    while(fgets(line, 1000, file_in)) {
        i++;
        result = realloc(result, (i + 1) * sizeof(char*));
        result[i - 1] = line;
        result[i - 1][strlen(line) - 1] = 0;
        line = calloc(1000, sizeof(char));
        if(line == NULL)
            errx(EXIT_FAILURE, "failed to allocate memory");
    }
    result[i] = NULL;

    free(line);
    fclose(file_in);

    return result;
}

char *file_to_str(char *path) {
    char **lines = file_readlines(path);

    char *result = calloc(1, sizeof(char));
    if(result== NULL)
        errx(EXIT_FAILURE, "failed to allocate memory");

    size_t c = 0;
    for (size_t i = 0; lines[i]; i++) {
        result = realloc(result, (c + strlen(lines[i]) + 1) * sizeof(char));
        for (size_t j = 0; lines[i][j]; j++) {
            result[c++] = lines[i][j];
        }
        free(lines[i]);
        result[c++] = '\n';
    }

    result[c - 1] = 0;

    free(lines);
    return result;
}

void file_write(char *path, char *data) {
    FILE *out;
    out = fopen(path, "w");

    if (out == NULL) {
        errx(EXIT_FAILURE, "%s", "File Not Found Error");
        return;
    }

    fprintf(out, "%s", data);
    fclose(out);
}

void file_writelines(char *path, char **data) {
    FILE *out;
    out = fopen(path, "w");

    if (out == NULL) {
        errx(EXIT_FAILURE, "%s", "File Not Found Error");
        return;
    }

    for (size_t i = 0; data[i]; i++) {
        for (size_t j = 0; data[i][j]; j++) {
            fprintf(out, "%c", data[i][j]);
        }
        fprintf(out, "%c", '\n');
    }

    fclose(out);
}
