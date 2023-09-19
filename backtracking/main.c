#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "filestream.h"
#include "allocator.h"

#define dim 11

int main(int argc, char **argv)
{
    if(argc != 2)
        errx(1, "give a right number of arguments");
    int **FinalGrid = allocGrid(dim);

    gridReader(dim, FinalGrid, argv[1]);
    for(size_t i = 0; i<dim; ++i)
    {
	for(size_t j = 0; j < dim; ++j)
	    printf("%i", FinalGrid[i][j]);
	printf("\n");
    }

    freeGrid(FinalGrid);
    return 0;
}
