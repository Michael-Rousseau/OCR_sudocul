void d_back_propagation(double **network,size_t *sizes, size_t layers, double **weight, double **biases, double learning_rate, double* target)
{
    for (size_t i = 0; i < sizes[layers - 1]; i++) {
        double output = network[layers - 1][i];
        double delta_output_layer = output - target[i];
        delta_output_layer *= 2;
        double cost_gradient = delta_output_layer * prime_sigmoid(output);
        biases[layers - 2][i] += cost_gradient;

        for (size_t j = 0; j < sizes[layers - 2]; j++) {
            weight[layers - 2][i * sizes[layers - 2] + j] += cost_gradient * network[layers - 2][j];
        }
    }

    for (size_t layer = layers - 2; layer > 0; layer--) {
        for (size_t i = 0; i < sizes[layer]; i++) {
            double output = network[layer][i];
            double delta_hidden_layer = 0.0;

            for (size_t j = 0; j < sizes[layer + 1]; j++) {
                delta_hidden_layer += weight[layer][i * sizes[layer + 1] + j] * prime_sigmoid(output) * biases[layer][j];
            }

            biases[layer - 1][i] += delta_hidden_layer;

            for (size_t j = 0; j < sizes[layer - 1]; j++) {
                weight[layer - 1][i * sizes[layer - 1] + j] += delta_hidden_layer * network[layer - 1][j];
            }
        }
    }

    for (size_t layer = 0; layer < layers - 1; layer++) {
        for (size_t i = 0; i < sizes[layer + 1]; i++) {
            biases[layer][i] -= learning_rate * biases[layer][i];
            for (size_t j = 0; j < sizes[layer]; j++) {
                weight[layer][i * sizes[layer] + j] -= learning_rate * weight[layer][i * sizes[layer] + j];
            }
        }
    }
}
