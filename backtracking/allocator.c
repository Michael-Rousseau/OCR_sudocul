#include <stdio.h>
#include <stdlib.h>
#include <err.h>
unsigned int **allocGrid(unsigned int dimension)
{
    unsigned int **grid = NULL;
    grid = calloc(dimension + 1, sizeof(unsigned int *));
    if (grid == NULL)
    {
        errx(EXIT_FAILURE, "Failing while allocating grid");
    }
    for (unsigned int i = 0; i < dimension; i++)
    {
        grid[i] = calloc(dimension + 1, sizeof(unsigned int));
        if (grid[i] == NULL)
        {
            errx(EXIT_FAILURE, "Failing while allocating grid");
        }
    }
    return grid;
}

void freeGrid(unsigned int **grid, unsigned int dimension)
{
    for (unsigned int i = 0; i < dimension; i++)
    {
        free(grid[i]);
    }
    free(grid);
}
