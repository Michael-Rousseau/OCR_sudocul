#include "helper.h"
#include "mnist.h"
#include "network.h"
#include "ocr.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t LAYER_COUNT = 3;

#define NUM_IMAGES 50000
#define LEARNING_RATE 0.9

double *load_image(char *filename) {
  FILE *file;
  char magic_number[3];
  int width, height, max_value, pixel;

  // Open the file for reading
  file = fopen(filename, "rb");
  if (file == NULL) {
    errx(1, "Error opening file");
    return NULL;
  }

  // Read and parse the header
  fscanf(file, "%s\n %d\n %d\n %d", magic_number, &width, &height, &max_value);
  if (magic_number[0] != 'P' ||
      (magic_number[1] != '2' && magic_number[1] != '5')) {
    printf("Invalid PGM format\n");
    fclose(file);
    return NULL;
  }

  // Allocate memory for the image
  double *image = (double *)malloc(width * height * sizeof(double));
  if (image == NULL) {
    errx(1, "Memory allocation failed");
    fclose(file);
    return NULL;
  }

  // Read the pixel data
  for (int i = 0; i < width * height; i++) {
    fscanf(file, "%d", &pixel);
    image[i] = pixel;
  }
  fclose(file);
  return image;
}


void load_and_train(network *n) {
  double *current_image;
  char filename[256];
  size_t rate = 0;
  size_t cpi = 0;
  int* target_output = calloc(10, sizeof(int));


  load_mnist();
  for (int i = 0; i < NUM_IMAGES; ++i) {
    int a = snprintf(filename, sizeof(filename), "./data/image_%d.pgm", i);
    if (a == -1)
      err(1, "Error writing in tab");

    current_image = load_image(filename);

    if (current_image == NULL)
      errx(1, "Didn't get the image from load");


    target_output[train_label[i]] = 1; 

    feed_forward(n, current_image);
    if (read_output(n) == train_label[i])
      rate++;

    back_prop(n, target_output);
    learn(n, LEARNING_RATE);

    target_output[train_label[i]] = 0;
    if (i % 10 == 0 && i != 0)
      printf("Accuracy at iteration %d: %.2f%%\n", i, (double)rate / i * 100.0);

    cpi = i;
  }

  printf("Final accuracy: %.2f%%\n", (double)rate / cpi * 100.0);
  free(target_output);
}

void build_Images(int i) {
  load_mnist();
  for (int a = 0; a < i; ++a) {
    save_mnist_pgm(train_image, a);
  }
}

int main() {
  size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
  layers[0] = 784;
  layers[1] = 1000;
  layers[2] = 10;
  network *n = xavier_init_network(layers, LAYER_COUNT);

//  build_Images(50000);
  load_and_train(n);

  free_network(n);
  return 0;
}
