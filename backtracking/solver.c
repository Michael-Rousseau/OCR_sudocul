#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int ValidRow(unsigned int **grid, unsigned int x,
        unsigned int val, unsigned int dim)
{
    for(size_t i = 0; i< dim; ++i)
    {
        if(grid[x][i] == val)
            return 0;
    }
    return 1;
}

unsigned int ValidColumn(unsigned int **grid, unsigned int y,
        unsigned int val, unsigned int dim)
{
    for(size_t i = 0; i<dim; ++i)
    {
        if(grid[i][y] == val)
            return 0;
    }
    return 1;
}

unsigned int ValidSquare(unsigned int **grid, unsigned int x, unsigned int y,
        unsigned int val, unsigned int dim)
{
    int where = (int)sqrt(dim);
    y-= y%where;
    x-= x%where;

    for(int i = 0; i<where; ++i)
    {
        for(int j = 0; j<where; ++j)
        {
            if(grid[x+i][y+j] == val)
                return 0;
        }
    }
    return 1;
}

unsigned int BePlaced(unsigned int **grid, unsigned int x, unsigned int y,
        unsigned int val, unsigned int dim)
{
    if(ValidRow(grid, x,val,dim) && ValidColumn(grid, y,val,dim)
            && ValidSquare(grid, x,y,val,dim))
        return 1;
    return 0;
}


unsigned int solve(unsigned int **grid, unsigned int x, unsigned int y,
        unsigned int dim)
{
    if (y>= (dim -2) && x > (dim -2))
        return 1;
    if(y >= (dim-2))
    {
        y = 0;
        ++x;
    }

    if(grid[x][y] != 0)
        return solve(grid, x, y+1, dim);
    for(size_t val = 1 ;val<= dim; ++val)
    {
        if(BePlaced(grid, x,y,val,dim))
        {
            grid[x][y] = val;
        }
        if(solve(grid, x,y+1, dim))
            return 1;
        grid[x][y] = 0;
    }
    return 0;
}
