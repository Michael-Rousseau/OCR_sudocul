#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "helper.h"
#include "network.h"

double softmax_denominator(double x, size_t total_classes) {
  double sum = 0.0;
  for (size_t i = 0; i < total_classes; ++i) {
    sum += exp(x);
  }
  return sum;
}

void softmax(unsigned int k, double *z) {
  double max_val = z[0];
  for (unsigned i = 1; i < k; i++) {
    if (z[i] > max_val) {
      max_val = z[i];
    }
  }

  double sum = 0;
  for (unsigned i = 0; i < k; i++) {
    sum += exp(z[i] - max_val);
  }

  sum = sum == 0 ? 1 : sum;
  for (unsigned i = 0; i < k; i++) {
    z[i] = exp(z[i] - max_val) / sum;
  }
}

double softmax_derivative(double softmax_value) {
  return softmax_value * (1.0 - softmax_value);
}

double sigmoid(double x) { return 1 / (1 + exp(-x)); }

double prime_sigmoid(double x) { return x * (1 - x); }
/*
void feed_forward(network *n, double *inputs) {
  for (size_t i = 0; i < n->layers[0]; i++)
    n->values[0][i] = inputs[i];

  for (size_t layer = 1; layer < n->len - 1; layer++) {
    double **wmat = n->weights[layer - 1];
    double *v_inp = n->values[layer - 1];
    double *v_out = n->values[layer];
    double *b_row = n->biases[layer - 1];

    for (size_t i = 0; i < n->layers[layer]; i++) {
      double *wrow = wmat[i];
      double sum = 0;

      for (size_t j = 0; j < n->layers[layer - 1]; j++) {
        sum += v_inp[j] * wrow[j];
      }

      // Apply ReLU activation
      v_out[i] = fmax(0, sum + b_row[i]);
    }
  }
  size_t last = n->len - 1;
  double *output_layer_input = n->values[last - 1];
  double *output_layer_output = n->values[last];
  double *output_layer_biases = n->biases[last - 1];

  for (size_t i = 0; i < n->layers[last]; i++) {
    double sum = output_layer_biases[i];

    for (size_t j = 0; j < n->layers[last - 1]; j++) {
      sum += n->weights[last - 1][i][j] * output_layer_input[j];
    }

    // Softmax (assuming the last layer is the output layer)
    output_layer_output[i] = exp(sum);
  }
}*/
void feed_forward(network *n, double *inputs) {
  for (size_t i = 0; i < n->layers[0]; i++)
    n->values[0][i] = inputs[i];
  for (size_t layer = 1; layer < n->len - 1; layer++) {
    double **wmat = n->weights[layer - 1];
    double *v_inp = n->values[layer - 1];
    double *v_out = n->values[layer];
    double *b_row = n->biases[layer - 1];

    for (size_t i = 0; i < n->layers[layer]; i++) {
      double *wrow = wmat[i];
      double sum = 0;

      for (size_t j = 0; j < n->layers[layer - 1]; j++) {
        sum += v_inp[j] * wrow[j];
      }

      v_out[i] = sigmoid(sum + b_row[i]);
    }
  }
  size_t last = n->len - 1;
  double *output_layer_input = n->values[last - 1];
  double *output_layer_output = n->values[last];
  double *output_layer_biases = n->biases[last - 1];

  for (size_t i = 0; i < n->layers[last]; i++) {
    double sum = output_layer_biases[i];

    for (size_t j = 0; j < n->layers[last - 1]; j++) {
      sum += n->weights[last - 1][i][j] * output_layer_input[j];
    }

    output_layer_output[i] = sum;
  }

  softmax(n->layers[last], output_layer_output);
}

size_t read_output(network *n) {
  size_t last = n->len - 1;

  int counter_null = 0;
  size_t max = 0;
  for (size_t i = 1; i < n->layers[last]; i++) {
    //    printf("last vaues[%zu] : %f\n", i, n->values[last][i]);
    if (n->values[last][i] == 0.0)
      counter_null++;
    if (n->values[last][i] > n->values[last][max])
      max = i;
    if (counter_null > 5)
      errx(1, "stop");
  }

  return max;
}

void back_prop(network *n, int *expected) {
  size_t last = n->len - 1;
  for (size_t i = 0; i < n->layers[last]; i++) {
    double v = n->values[last][i];
    n->costs[last - 1][i] = v * (1 - v) * (v - expected[i]);
  }
  for (size_t i = last - 1; i > 0; i--) {
    for (size_t j = 0; j < n->layers[i]; j++) {
      double sum = 0.0;
      for (unsigned k = 0; k < n->layers[i + 1]; k++) {
        sum += n->weights[i][k][j] * n->costs[i][k];
      }
      n->costs[i - 1][j] = sum * n->values[i][j] * (1 - n->values[i][j]);
    }
  }
}

void learn(network *n, double speed) {
  for (size_t i = 0; i < n->len - 1; i++) {
    for (size_t j = 0; j < n->layers[i]; j++) {
      for (size_t k = 0; k < n->layers[i + 1]; k++) {
        n->weights[i][k][j] -= speed * n->values[i][j] * n->costs[i][k];
      }
    }

    for (size_t j = 0; j < n->layers[i + 1]; j++) {
      n->biases[i][j] -= speed * n->costs[i][j];
    }
  }
  for (size_t i = 0; i < n->len - 1; i++) {
    for (size_t j = 0; j < n->layers[i + 1]; j++) {
      n->costs[i][j] = 0;
    }
  }
}
