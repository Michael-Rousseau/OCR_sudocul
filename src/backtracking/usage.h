#include "allocator.h"
#include "filestream.h"
#include "solver.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
  printf("Usage:\n");
  printf("./solver filename dim");
  errx(1, "give a right number of arguments");
}

