#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "filestream.h"
#include "allocator.h"

int main(int argc, char **argv)
{
    if(argc != 2)
        errx(1, "chef");
    unsigned int dim = 0;
    unsigned int **grid = allocGrid(dim);
    gridReader(dim, grid, argv[1]);

    for(size_t i = 0; i< dim; ++i)
        for(size_t j = 0; j < dim; ++j)
            printf("%i\n", grid[i][j]);
    freeGrid(grid, dim);
    return 0;
}
