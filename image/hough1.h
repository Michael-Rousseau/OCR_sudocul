#ifndef HOUGH1_H
#define HOUGH1_H

struct point {
    int x;
    int y;
};

int** hough_transform(int** edge_image, int image_height, int image_width, int *accumulator_height, int *accumulator_width);
int** create_accumulator(int accumulator_height, int accumulator_width);
void free_accumulator(int** accumulator, int accumulator_height);
void find_peaks(int** accumulator, int accumulator_height, int accumulator_width, struct point* peaks, int* num_peaks, int threshold);

#endif
