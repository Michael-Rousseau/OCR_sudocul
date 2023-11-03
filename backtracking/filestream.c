#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>


unsigned int cast(char t)
{
    switch(t)
    {
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            return t-'0';
    }


}
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
    unsigned int grid[dimension * dimension];
    size_t index = 0;
    while((car = fgetc(file)) != EOF)
    {
       if(car == '.')
        {
            grid[index] = 0;
        }
        else if((car > '0'  && car <='9') )
        {
            grid[index] = car -'0';
        }
        else if((car >= 'A' && car <= 'F'))
            grid[index] = cast(car);

        else if(car == '\0' || car == ' ')
        {
            grid[index] = -1;
        }
        else if(car != '\n')
        {
            errx(1, "FILE DOESN'T RESPECT THE FORMAT");
        }
        if(car != '\n' && car != '\0' && car != ' ')
           index++;
    }

    for(size_t i = 0; i < dimension; ++i)
    {
        for(size_t j = 0; j<dimension; ++j)
            FinalGrid[i][j] = grid[i * dimension +j];
    }
    fclose(file);
}


void gridWriter(unsigned int dim, unsigned int** FinalGrid, char* _path)
{
    double SqDim = sqrt(dim);
    FILE *file = fopen(_path, "w");
    if (file == NULL) {
        errx(1, "FILE OPEN FAILED");
    }
    for (size_t i = 0; i < dim; ++i)
    {
        if(i % 3 == 0 && i != 0)
            fprintf(file, "\n");
        for (size_t j = 0; j < dim; ++j)
        {
            if(j % (int)SqDim == 0 && j !=0)
                fprintf(file," %u", FinalGrid[i][j]);
            else
                fprintf(file, "%u", FinalGrid[i][j]);
            if (j == dim - 1) {
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}
