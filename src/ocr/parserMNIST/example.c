#include "mnist.h"
#include <err.h>
#include <string.h>

int main(int argc, char **argv) {
  // call to store mnist in array
  size_t NUM_IMAGE;
  if (argc != 2)
    errx(1, "wrong number of parameters");
  NUM_IMAGE = atoi(argv[1]);
  load_mnist();
  // give:
  //
  // train_image = array of 60000 images of
  // 784 px
  //
  // train_label = index is the number of train_image
  //
  // test_image = array of 10000 images of 784 px
  //
  // TEST_LABEL = index is the number of test_image

  for (size_t i = 0; i <= NUM_IMAGE; ++i) {
    save_mnist_pgm(test_image, i);
  }
  return 0;
}
