#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>

void gridReader(unsigned int dimension, unsigned int** FinalGrid, char _path[])
{
    FILE *file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        //TODO: errx
        return;
    }
    char car;
    unsigned int grid[dimension * dimension];
    size_t index = 0;
    while((car = fgetc(file)) != EOF)
    {
        if(car == '.')
        {
            grid[index] = 0;
        }
        else if(car > '0'  && car <'9')
        {
            grid[index] = car -'0';
        }
        else if(car >= 'A' && car <= 'Z')
        {
            grid[index] = car - 'A' + 10;
        }
        else if (car >= 'a' && car <= 'z')
        {
            grid[index] = car - 'a' + 10;
        }
        else if(car != '\n' && car != '\0' && car != ' ')
        {
            errx(1, "FILE DOESN'T RESPECT THE FORMAT");
        }
        else
        {
            continue;
        }
        index++;
    }
    for(size_t i = 0; i < dimension; ++i)
    {
        for(size_t j = 0; j < dimension; ++j)
        {
            FinalGrid[i][j] = grid[i * dimension + j];
        }
    }
    fclose(file);
}
