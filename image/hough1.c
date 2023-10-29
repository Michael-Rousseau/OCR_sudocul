#include "hough1.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DEGREE 0.017453293 // Conversion factor from degrees to radians

int** create_accumulator(int accumulator_height, int accumulator_width) {
    // Allocate memory for accumulator
    int **accumulator = malloc(accumulator_height * sizeof(int*));
    if (accumulator == NULL) {
        fprintf(stderr, "Unable to allocate memory for the accumulator.\n");
        exit(1); // FAILED
    }

    for (int i = 0; i < accumulator_height; i++) {
        accumulator[i] = calloc(accumulator_width, sizeof(int)); // Using calloc to initialize to zero
        if (accumulator[i] == NULL) {
            fprintf(stderr, "Unable to allocate memory for row %d in the accumulator.\n", i);
            exit(1); // FAILED
        }
    }
    return accumulator;
}

void free_accumulator(int** accumulator, int accumulator_height) {
    for (int i = 0; i < accumulator_height; i++) {
        free(accumulator[i]);
    }
    free(accumulator);
}

int** hough_transform(int** edge_image, int image_height, int image_width, int *accumulator_height, int *accumulator_width) {
    int max_dist = (int)sqrt(image_width * image_width + image_height * image_height);
    *accumulator_height = 2 * max_dist;
    *accumulator_width = 180;

    int **accumulator = create_accumulator(*accumulator_height, *accumulator_width);


    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            if (edge_image[y][x] != 0) {  // If we have an edge pixel
                for (int theta = 0; theta < *accumulator_width; theta++) {
                    double rad = DEGREE * theta; // Convert degree to radian
                    int r = (int)(x * cos(rad) + y * sin(rad)); // r = x*cos(theta) + y*sin(theta)
                    r += max_dist;
                    accumulator[r][theta] += 1; // Vote in the accumulator
                }
            }
        }
    }

    return accumulator;
}

void find_peaks(int** accumulator, int accumulator_height, int accumulator_width, struct point* peaks, int* num_peaks, int threshold) {
    *num_peaks = 0;

    for (int y = 1; y < accumulator_height - 1; y++) {
        for (int x = 1; x < accumulator_width - 1; x++) {
            int current = accumulator[y][x];
            if (current >= threshold &&
                current > accumulator[y - 1][x] &&
                current > accumulator[y + 1][x] &&
                current > accumulator[y][x - 1] &&
                current > accumulator[y][x + 1]) {
                // This is a local maximum
                peaks[*num_peaks].x = x;
                peaks[*num_peaks].y = y;
                (*num_peaks)++;
            }
        }
    }
}


