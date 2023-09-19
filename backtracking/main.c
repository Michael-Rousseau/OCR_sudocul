#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "filestream.h"
#include "allocator.h"

int main(int argc, char **argv)
{
    if(argc != 2)
        errx(1, "give a right number of arguments");
    unsigned int dim = 11;
    int **grid = allocGrid(dim);

    gridReader(dim, grid, argv[1]);

    freeGrid(grid);
    return 0;
}
