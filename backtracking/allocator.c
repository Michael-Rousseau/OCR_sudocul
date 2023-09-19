#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int **allocGrid(unsigned int dimension)
{
    int **grid = NULL;
    grid = calloc(dimension, sizeof(int));
    //the calloc function will create a new dynamic tab in memory -> next courses

    if (grid == NULL)
    {
        errx(EXIT_FAILURE, "Failing while allocating grid");
    }
    for (size_t i = 0; i < dimension; i++)
    {
        grid[i] = calloc(dimension, sizeof(int));
        if (grid[i] == NULL)
        {
            errx(EXIT_FAILURE, "Failing while allocating grid");
        }
    }
    return grid;
}

void freeGrid(int **grid)
{
    //this function will be used everywhere to free an array

    free(grid);
}
