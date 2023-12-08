#define _GNU_SOURCE
#include "helper.h"
#include "mnist.h"
#include "network.h"
#include "ocr.h"

#include "readSDL.h"
#include <emmintrin.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t LAYER_COUNT = 3;
#define NUM_IMAGES 81
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
      size_t end = start + batch_size;
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
        if ((start + i + 1) % 10 == 0) {
          double accuracy_so_far = (double)correct_predictions / (10.0);
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

  /*
  const char *folder =
      "./data/digits_im/0/"; // Replace with the actual folder path
  char **pngFiles;
  int numFiles;

  get_png_files(folder, &pngFiles, &numFiles);

  // Length of images dimensions:
  // images[9 (label)][NB_IMAGES][784]
  double ***images;
  load_images(&images, pngFiles, NUM_IMAGES);
*/
  int label_first[] = {5, 3, 0, 0, 7, 0, 0, 0, 6, 0, 0, 1, 9, 5, 0, 0,
                       0, 0, 9, 8, 0, 0, 0, 0, 6, 0, 8, 0, 0, 0, 6, 0,
                       0, 0, 3, 4, 0, 0, 8, 0, 3, 0, 0, 1, 7, 0, 0, 0,
                       2, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 2, 8, 0, 0, 0,
                       0, 4, 1, 9, 0, 0, 5, 0, 0, 0, 0, 8, 0, 0, 7, 9};

  int label_second[] = {0, 2, 0, 0, 0, 0, 6, 0, 9, 8, 5, 7, 0, 6, 4, 2, 0,
                        0, 0, 9, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 6, 5, 0, 3,
                        0, 0, 0, 0, 8, 1, 0, 3, 5, 0, 0, 0, 0, 3, 0, 2, 9,
                        0, 8, 0, 0, 0, 0, 4, 0, 0, 0, 6, 0, 0, 0, 2, 8, 7,
                        0, 1, 3, 5, 1, 0, 6, 0, 0, 0, 0, 2, 0};
  int label_third[] = {
      0, 0, 0, 0, 0, 4, 5, 8, 0, 0, 0, 0, 7, 2, 1, 0, 0, 3, 4, 0, 3,
      0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 6, 7, 0, 0, 4, 0, 7, 0, 0, 0, 0,
      2, 0, 0, 6, 3, 0, 0, 4, 9, 0, 0, 1, 0, 0, 0, 0, 0, 6, 9, 5, 0,
  };
  int i = 0;

  //  for (int label = 0; label < 9; ++label) {
  //  for (int num = 0; num < 30; num++) {
  //     double *current_image = images[label][i];
  // int *target_output = calloc(10, sizeof(int));
  // target_output[label] = 1;

  //   inputs[i] = current_image;
  // targets[i++] = target_output;

  // free(target_output);
  //}
  //}
  size_t num_samples = 81;
  size_t num_epochs = 1;
  size_t batch_size = 1;

  double *pixels = malloc(28 * 28 * sizeof(double));
  char path[100];
  int *label = calloc(81, sizeof(int));

  for (int i = 0; i < 81; ++i) {
    snprintf(path, sizeof(path), "data/tst/firstimg/square_%d.bmp", i);
    printf("%s", path);
    get_tab(path, pixels);
    label[label_first[i]] = 1;
    targets[i] = label;
    inputs[i] = pixels;
    train(n, LEARNING_RATE, inputs, targets, num_samples, num_epochs,
          batch_size);
    label[label_first[i]] = 0;
  }

  for (int i = 0; i < num_samples; ++i) {
    free(targets[i]);
  }
  free(inputs);
  free(targets);
  // free_file_names(&pngFiles, numFiles);
  // free_images(&images, numFiles);
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
    for (int j = 0; j < 784; ++j) {
      if (inputs[i] > 0.5)
        inputs[i] = 1;
      else
        inputs[i] = 0;
    }
    feed_forward(n, inputs);
    if (read_output(n) == expected) {
      accuracy++;
    }
    printf("result: %zu\nexpected: %d\n", read_output(n), expected);
  }
  double final = (double)accuracy / (double)nb_test;
  printf("final : %f\n", final * 100);
}
void test_from_file()
{
    network *n = import_network("97network.nw");
    const char* filename = "uncinq.bmp";
    double* pixels = malloc(784 * sizeof(double));
    get_tab(filename,pixels);
    feed_forward(n,pixels);
    printf("expected:5\ngot:%zu\n", read_output(n));

    free(pixels);

}
int main() {
  size_t *layers = malloc(LAYER_COUNT * sizeof(size_t));
  layers[0] = 784;
  layers[1] = 300;
  layers[2] = 10;
  network *n = xavier_init_network(layers, LAYER_COUNT);

  // first: call build_images and then comment it

  //test_from_file();
  // build_Images(NUM_IMAGES);
  load_and_train(n);

  export_network(n, "networkOwnData.nw");
  // test_from_load(1000);

  free_network(n);
  return 0;
}
