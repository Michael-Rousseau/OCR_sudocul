#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>

void gridReader(unsigned int dimension, int** FinalGrid, char* _path)
{
    //the first part of the algorithm will read the grid FILE
    //and after, transform the array into 2 array dim

    FILE *file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        errx(EXIT_FAILURE, "%s\n", "No file found");
        return;
    }
    char car;
    int grid[dimension * dimension];
    size_t index = 0;
    while((car = fgetc(file)) != EOF)
    {
        
       if(car == '.')
        {
            grid[index] = 0;
        }
        else if(car > '0'  && car <='9')
        {
            grid[index] = car -'0';
        }
        else if(car == '\0' || car == ' ')
        {
            grid[index] = -1;
        }
        else if(car != '\n')
        {
            errx(1, "FILE DOESN'T RESPECT THE FORMAT");
        }
        if(car != '\n' && car != '\0')
           index++;
    }

    for(size_t i = 0; i < dimension; ++i)
    {
        for(size_t j = 0; j<dimension; ++j)
            printf("%i ",grid[i*dimension +j]);
        printf("\n");
    }
    fclose(file);
    return;
    FinalGrid[1][2] = 0;
}

