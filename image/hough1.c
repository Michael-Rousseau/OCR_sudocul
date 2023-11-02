#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "hough1.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DEGREE 0.017453293 // Conversion factor from degrees to radians
#define MAX_THETA 180
#define PI_2 (3.14159265358979323846 / 2) // π/2 if not defined in math.h



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
/*
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
*/



int** hough_transform(int** edge_image, int image_height, int image_width, int *accumulator_height, int *accumulator_width) {
    int max_dist = (int)sqrt(image_width * image_width + image_height * image_height);
    *accumulator_height = 2 * max_dist; // accommodate negative and positive r values
    *accumulator_width = MAX_THETA; // 180 degrees

    // Check if the allocations were successful before proceeding
    int **accumulator = create_accumulator(*accumulator_height, *accumulator_width);
    if (!accumulator) {
        // handle the error, for instance, print a message and terminate the program
        fprintf(stderr, "Failed to allocate memory for accumulator.\n");
        exit(EXIT_FAILURE); // or any appropriate error handling
    }

    // Perform the Hough Transform
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            if (edge_image[y][x] != 0) { // if it's an edge
                for (int theta = 0; theta < MAX_THETA; theta++) {
                    double rad = DEGREE * theta;
                    int r = (int)(x * cos(rad) + y * sin(rad)) + max_dist; // shifted to ensure it's non-negative
                    if (r >= 0 && r < *accumulator_height) {
                        accumulator[r][theta]++; // vote
                    }
                }
            }
        }
    }

    return accumulator;
}


