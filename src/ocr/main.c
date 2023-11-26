#include "helper.h"
#include "network.h"
#include "ocr.h"
#include "parserMNIST/mnist.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t LAYER_COUNT = 3;

#define NUM_IMAGE 200
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
  fscanf(file, "%s %d %d %d", magic_number, &width, &height, &max_value);
  if (magic_number[0] != 'P' ||
      (magic_number[1] != '2' && magic_number[1] != '5')) {
    printf("Invalid PGM format\n");
    fclose(file);
    return NULL;
  }

  // Allocate memory for the image
  printf("magic_number :%s\n", magic_number);
  printf("width: %d\n height: %d\n max_value: %d\n", width, height, max_value);
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

void create_and_save_PGM() {

  load_mnist();
  for (size_t i = 0; i <= NUM_IMAGE; ++i) {
    save_mnist_pgm(test_image, i);
  }
}

int right_number(network *n, size_t i) {
  load_mnist();
  // TODO: maybe apply softmax here
  return n->values[n->len - 1][test_label[i]] == 1;
}

void load_and_train(network *n) {
  double *current_image;
  char filename[256];
  // load_mnist();
  size_t rate = 0;
  size_t cpi = 0;
  for (int i = 0; i <= NUM_IMAGE; ++i) {
    int a = snprintf(filename, sizeof(filename), "image_%i.pgm", i);
    if(a == -1)
        err(1, "error writing in tab");
    current_image = load_image(filename);
    if (current_image == NULL)
      errx(1, "didn't get the image from load");
    feed_forward(n, current_image);
    if (right_number(n, i))
      rate++;
    back_prop(n, current_image);
    learn(n, LEARNING_RATE);
    if (i % 10 == 0 && i != 0)
      printf("rate: %zu\n", rate / i);
    cpi = i;
    free(current_image);
  }
  printf("final rate: %zu\n", rate / cpi);
}

int main() {
  size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
  layers[0] = 784;
  layers[1] = 16;
  layers[2] = 10;
  network *n = rand_init_network(layers, LAYER_COUNT, -1, -1, -1, -1);
  create_and_save_PGM();

  load_and_train(n);

  free_network(n);
  return 0;
}
