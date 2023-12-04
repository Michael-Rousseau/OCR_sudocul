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
#define LEARNING_RATE 0.1

void train_batch(network *n, double speed, double **batch_inputs,
                 int **batch_targets, size_t batch_size) {
  for (size_t batch = 0; batch < batch_size; ++batch) {
    feed_forward(n, batch_inputs[batch]);
    back_prop(n, batch_targets[batch]);
    learn(n, speed);
  }
}

void train(network *n, double speed, double **inputs, int **targets,
           size_t num_samples, size_t num_epochs, size_t batch_size) {
  load_mnist();
  for (size_t epoch = 0; epoch < num_epochs; ++epoch) {
    size_t correct_predictions = 0;

    size_t index = 0;
    for (size_t start = 0; start < num_samples; start += batch_size) {
      size_t end = start +batch_size ;
      if (end > num_samples) {
        end = num_samples;
      }

      size_t current_batch_size = end - start;
      double **batch_inputs = &inputs[start];
      int **batch_targets = &targets[start];
      train_batch(n, speed, batch_inputs, batch_targets, current_batch_size);
      for (size_t i = 0; i < current_batch_size; ++i) {
        feed_forward(n, batch_inputs[i]);
        if ((int)read_output(n) == train_label[index++])
          correct_predictions++;
        if ((start + i + 1) % 1000 == 0) {
          double accuracy_so_far =
              (double)correct_predictions / (10.0);
          printf("Epoch %zu - Tests: %zu - Accuracy: %.2f%%\n Correct: %zu\n",
                 epoch + 1, start + i + 1, accuracy_so_far,
                 correct_predictions);
          correct_predictions = 0;
        }
      }
    }
    double accuracy = (double)correct_predictions / num_samples * 100.0;
    printf("Epoch %zu - Final Accuracy: %.2f%%\n", epoch + 1, accuracy);
  }
}
double *load_image(char *filename) {
  FILE *file;
  char magic_number[3];
  int width, height, max_value, pixel;
  file = fopen(filename, "rb");
  if (file == NULL) {
    errx(1, "call build_Images");
    return NULL;
  }
  fscanf(file, "%s\n %d\n %d\n %d", magic_number, &width, &height, &max_value);
  if (magic_number[0] != 'P' ||
      (magic_number[1] != '2' && magic_number[1] != '5')) {
    printf("Invalid PGM format\n");
    fclose(file);
    return NULL;
  }
  double *image = (double *)malloc(width * height * sizeof(double));
  if (image == NULL) {
    errx(1, "Memory allocation failed");
    fclose(file);
    return NULL;
  }
  for (int i = 0; i < width * height; i++) {
    fscanf(file, "%d", &pixel);
    image[i] = pixel;
  }
  fclose(file);
  return image;
}

void load_and_train(network *n) {
  double **inputs = malloc(NUM_IMAGES * sizeof(double *));
  int **targets = malloc(NUM_IMAGES * sizeof(int *));

  load_mnist();

  for (int i = 0; i < NUM_IMAGES; ++i) {
    char filename[256];
    int a = snprintf(filename, sizeof(filename), "./data/image_%d.pgm", i);
    if (a == -1)
      err(1, "Error writing in tab");

    double *current_image = train_image[i];
    for (size_t i = 0; i < 784; ++i)
    {
        if(current_image[i] > 0.5)
            current_image[i] = 1;
        else
         current_image[i] = 0;
    }


    if (current_image == NULL)
      errx(1, "Didn't get the image from load");

    int *target_output = calloc(10, sizeof(int));
    target_output[train_label[i]] = 1;

    inputs[i] = current_image;
    targets[i] = target_output;
  }

  size_t num_samples = NUM_IMAGES;
  size_t num_epochs = 10;
  size_t batch_size = 10;

  train(n, LEARNING_RATE, inputs, targets, num_samples, num_epochs, batch_size);

  for (int i = 0; i < NUM_IMAGES; ++i) {
    free(targets[i]);
  }
  free(inputs);
  free(targets);
}

void build_Images(int i) {
  load_mnist();
  for (int a = 0; a < i; ++a) {
    save_mnist_pgm(train_image, a);
  }
}

void test_from_load(int nb_test) {
  network *n = import_network("testnetwork");
  load_mnist();
  int accuracy = 0;
  for (int i = 0; i < nb_test; ++i) {
    int expected = test_label[i];

    double *inputs = test_image[i];
    for (int j = 0; j < 784; ++j)
    {
        if(inputs[i] >0.5)
            inputs[i] = 1;
        else
         inputs[i] = 0;
    }
    feed_forward(n, inputs);
    if(read_output(n) == expected)
    {
        accuracy++;
    }
    printf("result: %zu\nexpected: %d\n", read_output(n), expected);
  }
  double final = (double)accuracy / (double)nb_test; 
  printf("final : %f\n", final * 100);
}
int main() {
  size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
  layers[0] = 784;
  layers[1] = 300;
  layers[2] = 10;
  network *n = xavier_init_network(layers, LAYER_COUNT);

  // first: call build_images and then comment it

  // build_Images(NUM_IMAGES);
 load_and_train(n);

   export_network(n, "testnetwork");
  test_from_load(1000);

  free_network(n);
  return 0;
}
