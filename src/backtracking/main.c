#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "solver.h"
#include "filestream.h"
#include "allocator.h"


void usage()
{
    printf("Usage:\n");
    printf("./solver filename dim");
    errx(1, "give a right number of arguments");
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        usage();
    }
    size_t dim;
    if(strcmp(argv[2], "16") == 0)
        dim = 16;
    else
        dim =9;
    unsigned int **FinalGrid = allocGrid(dim);
//    char grid[100];
  //  snprintf(grid, sizeof(grid), "./data/%s", argv[1]);

    //gridReader(dim, FinalGrid, grid);
    gridReader(dim, FinalGrid, argv[1]);
    solve(FinalGrid,0,0,dim);
    char* name = argv[1];
    strcat(name, ".result");
    gridWriter(dim, FinalGrid, name);
    freeGrid(FinalGrid, dim);
    printf("generated file: %s\n", name);
    return 0;
}
