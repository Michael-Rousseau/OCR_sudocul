#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "detection.h"
#include <math.h>


#define true 1
#define false 0


float distance(struct Point a, struct Point b) 
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int thelinebelongs(struct Line l1, struct Line l2) 
{

	//if (fabs(l1.rho - l2.rho) <= rhoThreshold && fabs(l1.theta - l2.theta) <= thetaThreshold)

	float startDistance = distance(l1.start, l2.start);
	float endDistance = distance(l1.end, l2.end);

	if (startDistance < startThreshold && endDistance < endThreshold)
		return 1;
	return 0;
} 

void addtogroup(struct Line line, struct Linegroup* group) 
{
	group->average.start.x = (group->average.start.x * group->numlines + line.start.x) / (group->numlines + 1);
	group->average.start.y = (group->average.start.y * group->numlines + line.start.y) / (group->numlines + 1);
	group->average.end.x = (group->average.end.x * group->numlines + line.end.x) / (group->numlines + 1);
	group->average.end.y = (group->average.end.y * group->numlines + line.end.y) / (group->numlines + 1);
	group->numlines +=1;
}

int averagelines(struct Line* line, int len,  struct Linegroup** group) {
	int n = 10;
	int ncurrent = 0;

	for(int i = 0; i < len; i++) 
	{
		int loner = true;
		for(int j = 0; j < ncurrent; j++) 
		{
			if(thelinebelongs(line[i], group[j]->average) == 1)
			{
				addtogroup(line[i], group[j]);
				loner = false;
			}
		}
		if (loner) 
		{
			if(ncurrent == n) 
			{
				n *= 2;
				group = realloc(group, n * sizeof(struct Linegroup*));

				if (!group) 
				{
					fprintf(stderr, "Memory allocation failed.\n");
					exit(EXIT_FAILURE);
				}

				for (int j = ncurrent; j < n; j++) 
				{
					group[j] = NULL;
				}
			}
			group[ncurrent] = calloc(1, sizeof(struct Linegroup));
			if (!group[ncurrent]) 
			{
				fprintf(stderr, "Memory allocation failed.\n");
				exit(EXIT_FAILURE);
			}
			group[ncurrent]->average.start.x = line[i].start.x;	
			group[ncurrent]->average.start.y = line[i].start.y;

			group[ncurrent]->average.end.x = line[i].end.x;	
			group[ncurrent]->average.end.y = line[i].end.y;

			group[ncurrent]->numlines = 1;
			ncurrent++;
		}
	}
	return ncurrent;

}

void drawl(struct Line* line, int len, SDL_Renderer* renderer)
{
	struct Linegroup** group = calloc(len, sizeof(struct Linegroup*)); // use len as it represents the number of lines
	if (!group) {
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	int nmax = averagelines(line, len, group);

	for(int i = 0; i < nmax; i++)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Red color for the lines
		SDL_RenderDrawLine(renderer, group[i]->average.start.x, group[i]->average.start.y, 
				group[i]->average.end.x, group[i]->average.end.y);
	}
	for (int i = 0; i < nmax; i++)
		free(group[i]);
	free(group);
}
























/*
float distance(struct Point a, struct Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}


int thelinebelongs (struct Line l1, struct Line l2)
{
    if (fabs(l1.rho - l2.rho) < rhoThreshold && fabs(l1.theta - l2.theta) < thetaThreshold)
	    return 1;
    return 0;
} 

void addtogroup(struct Line line, struct Linegroup*  group)
{
	group->average.start.x =(group->average.start.x * group.numlines + line.start.x) / (group.numlines + 1);
	group->average->star =(group->average->start->y * group->numlines + line->start->y) / (group->numlines + 1);
	group->average.end- =(group->average.end * group.numlines + line.end) / (group.numlines + 1);
	group->average->end->y =(group->average->end->y * group->numlines + line->end->y) / (group->numlines + 1);
	group->numlines +=1;
}


int averagelines( struct Line* line,int len, SDL_Surface* image, struct Linegroup** group) 
{

	int n = 10;
	//Linegroup** group = calloc(n,sizeof(struct Linegroup*));
	int ncurrent = 0;

	for(int i = 0; i < len; i++)
	{
		int loner = true; //0 if the line does not have a group
		for(int j = 0; j <=ncurrent; j++)
		{
			if(thelinebelongs( line[i], group[j]) == 1)
			{
				addtogroup(line[i], group[j]);
				loner = false;
			}
			
		}
		if (loner)
		{
			if(ncurrent == n)
			{
				n*=2;
				group = realloc(group, n* sizeof(struct Linegroup*));
			}
			group[ncurrent] = calloc(1, sizeof(struct Linegroup));
			group[ncurrent]->average.start = line[i].star;
			group->average->start->y = line[i]->start->y;
			group[ncurrent]->averag.>end = line[i].end;
			group->average->end->y = line[i]->end->y;
			group->numlines = 1;
			ncurrent++;

		}
	}
	return ncurrent;
}



void drawl(SDL_Surface*image, struct Line* line, int len,SDL_Renderer* renderer)
{	struct Linegroup** group = calloc(n,sizeof(struct Linegroup*));

	int nmax = averagelines(line, len, image,group);

		for(int i = 0; i < nmax; i++)
		{

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Red color for the lines
			SDL_RenderDrawLine(renderer, group[i]->average.start.x, group[i]->average.start.y,group[i]->average.end.x, group[i]->average.end.y);
		}
}*/
