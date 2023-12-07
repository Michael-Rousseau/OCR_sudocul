#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include "hough.h"

//noir 0 
//blanc 1


const int theta_s = 180; // Adjust as needed for precision
const double rho = 1; // Distance resolution in pixels
const double theta = M_PI / theta_s; // Angle resolution in radians


//HOUGH ALGO

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return 0;  // Out of bounds

	Uint32 *pixels = (Uint32 *)surface->pixels;
	Uint32 pixel_value = pixels[y * surface->w + x];

	return pixel_value;
}



int accu_maxvalue(int ** accumulator,int rows, int cols)
{
	int maxVal = accumulator[0][0];

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (accumulator[i][j] > maxVal) {
				maxVal = accumulator[i][j];
			}
		}
	}

	return maxVal;
}


int isNearZero(double value, double epsilon) {
    if (fabs(value) < epsilon || fabs(value - M_PI) < epsilon ||
            fabs(value - M_PI_2) < epsilon)
	    return 1;
    return 0;
}


struct DetectedLines performHoughTransform(SDL_Surface *surface)
{
	const double DIAGONAL = sqrt(surface->w * surface->w+
                surface->h * surface->h);

	const int RHO_MAX = (int)(2 * DIAGONAL ) +1;

	const int RHO_OFFSET = RHO_MAX / 2;


	int w = surface->w;
	int h = surface->h;
	int n = 50;
	//create pointer of lines
	struct Line* lines = calloc(n ,sizeof(struct Line));


	//create accumulator
	int ** accumulator = malloc(RHO_MAX * sizeof(int*));
	for (int i = 0; i < RHO_MAX; i++)
		accumulator[i] = calloc(theta_s, sizeof(int));


	if (SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) != 0) {
		// Handle the error, perhaps log it or exit
		fprintf(stderr, "Could not lock surface: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	// Iterate over the pixels to collect votes
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			Uint8 r,g,b;
			Uint32 pixelvalue = get_pixel(surface, x, y);
			SDL_GetRGB(pixelvalue, surface->format, &r, &g, &b);


			if (r == 255 && g == 255 && b == 255) { //or r>128
				for (int t = 0; t < theta_s; t++)
				{
					double currentTheta = t * theta;
					double rho = (x * cos(currentTheta ))
                                            + (y * sin(currentTheta));

					int rhoIndex = (int)(rho + RHO_OFFSET);

					if (rhoIndex >= 0 && rhoIndex < RHO_MAX)
					{
						accumulator[rhoIndex][t]++;
					}

				}
				}

			}
		}


		if (SDL_MUSTLOCK(surface)) {
			SDL_UnlockSurface(surface);
		}

		int maxval = accu_maxvalue(accumulator, RHO_MAX, theta_s);


		// The threshold will depend on your specific
                // application and image characteristics
		int lineindex= 0;
		const int THRESHOLD = maxval * 0.2;


		for (int r = 0; r < RHO_MAX; r++)
		{
			for (int t = 0; t < theta_s; t++)
			{
				//printf("%i",accumulator[r][t]);
				if (accumulator[r][t] > THRESHOLD)
				{
					double foundRho = (r - RHO_OFFSET);
					double foundTheta = t * theta;
				//	float epsilon = 0.01;

					int x1, y1, x2, y2;
					if ( foundTheta< 0.01 ||fabs(foundTheta - M_PI)
                        < 0.01){ //works perfectly //near 0 or pi

				// Line is approximately vertical
						x1 = x2 = foundRho / cos(foundTheta);
						y1 = 0;
						y2 = h;

						if (x1 < 0) x1=x2=0;
						if (x1 >= w) x1 = x2 = w-1;


					} else if  ( fabs(foundTheta - M_PI/2 ) < 0.01) {
				    // Line is approximately horizontal

						y1 = y2 = (foundRho/ sin (foundTheta ));
						x1 = 0;
						x2 = w;

						if (y1 < 0) y1 = y2 = 0;
						if (y1 >= h) y1 = y2 = h - 1;

						//x1=x2=y1=y2=0;


					} else {
						// diagonal
						x1 = 0;
						y1 = foundRho / sin(foundTheta) ;
						x2 = w;
						y2 = ((foundRho - x2 * cos(foundTheta)) / sin(foundTheta) );

												x1=x2=y1=y2=0;

					}

					if (lineindex == n) {
						n *= 2;
						lines = (struct Line*) realloc(lines,
                                    n * sizeof(struct Line));
					}

					lines[lineindex].start.x = x1;
					lines[lineindex].end.x = x2;
					lines[lineindex].start.y = y1;
					lines[lineindex].end.y = y2;
					lines[lineindex].theta = t;
					lines[lineindex].rho = foundRho;
					lineindex++;



			}}
		}
		for (int i = 0; i < RHO_MAX; i++) {
			free(accumulator[i]);
		}
		free(accumulator);


		struct Line* temp = (struct Line*) realloc(lines,
                        lineindex * sizeof(struct Line));
		if (temp == NULL) {
			// Handle memory allocation failure
		}
		lines = temp;

		//printf("%i",lineindex);
		struct DetectedLines result;
		result.lines = lines;
		result.count = lineindex;

		return result;

		//return lines;

	}




//CALCULATE AVERAGE OF THE LINES


float distance(struct Point a, struct Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int thelinebelongs(struct Line l1, struct Line l2)
{

	//if (fabs(l1.rho - l2.rho) <= rhoThreshold && fabs(l1.theta -
        //l2.theta) <= thetaThreshold)

	float startDistance = distance(l1.start, l2.start);
	float endDistance = distance(l1.end, l2.end);

	if (startDistance < startThreshold && endDistance < endThreshold)
		return 1;
	return 0;
}

void addtogroup(struct Line line, struct Linegroup* group)
{
	group->average.start.x = (group->average.start.x * group->numlines
                + line.start.x) / (group->numlines + 1);
	group->average.start.y = (group->average.start.y * group->numlines
                + line.start.y) / (group->numlines + 1);
	group->average.end.x = (group->average.end.x * group->numlines
                + line.end.x) / (group->numlines + 1);
	group->average.end.y = (group->average.end.y * group->numlines
                + line.end.y) / (group->numlines + 1);
	group->average.theta = line.theta;
		group->average.rho = line.rho;
	group->numlines +=1;
}



int averagelines(struct Line* line, int len,  struct Linegroup** group) {
	int n = 100;
	int ncurrent = 0;

	for(int i = 0; i < len; i++)
	{
		int loner = 1; //true = 1
		for(int j = 0; j < ncurrent; j++)
		{
			if(thelinebelongs(line[i], group[j]->average) == 1)
			{
				addtogroup(line[i], group[j]);
				loner = 0; //false
			}
		}
		if (loner == 1)
		{
			if(ncurrent == n)
			{
				n *= 2;
				group = realloc(group, n *
                                        sizeof(struct Linegroup*));

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

			group[ncurrent]->average.theta = line[i].theta;
			group[ncurrent]->average.rho = line[i].rho;

			group[ncurrent]->numlines = 1;
			ncurrent++;
		}
	}
	return ncurrent;

}



void drawl(struct Line* line, int len, SDL_Renderer* renderer,
        SDL_Texture* texture)
{
	struct Linegroup** group = calloc(len, sizeof(struct Linegroup*));
        // use len as it represents the number of lines
	if (!group) {
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	int nmax = averagelines(line, len, group);

	  //  SDL_RenderClear(renderer);


//	SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

		int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (render != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	for(int i = 0; i < nmax; i++)
	{
		printf("start x %f y %f, end x%f y %f", group[i]->
                        average.start.x, group[i]->average.start.y,
				group[i]->average.end.x, group[i]->
                                average.end.y);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0,
                        SDL_ALPHA_OPAQUE); // Red color for the lines
		SDL_RenderDrawLine(renderer, group[i]->average.start.x,
                        group[i]->average.start.y,
				group[i]->average.end.x, group[i]->
                                average.end.y);
	}
	SDL_RenderPresent(renderer);

	for (int i = 0; i < nmax; i++)
		free(group[i]);
	free(group);


}



struct DetectedLines averagearray(struct Line* Line, int len)
{
	struct Linegroup** group = calloc(len, sizeof(struct Linegroup*));
        // use len as it represents the number of lines
	if (!group) {
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	int nmax = averagelines(Line, len, group);

	struct DetectedLines result;
	result.lines = calloc(nmax, sizeof(struct Line));
	result.count = nmax;
	//printf("nmax %i \n", nmax);

	for(int i = 0; i < nmax; i++)
	{
		//if (isPartOfSudoku(group,  nmax, group[i])) {

		result.lines[i].start.x = group[i]->average.start.x;
		result.lines[i].start.y = group[i]->average.start.y;
		result.lines[i].end.x = group[i]->average.end.x;
		result.lines[i].end.y = group[i]->average.end.y;
		result.lines[i].rho = group[i]->average.rho;
		result.lines[i].theta = group[i]->average.theta;
	
		//printf("nmax %i\n x %f",j , result.lines[j].start.x);

		//}

		free(group[i]);

	}
	free(group);

	return result;
}

//DETECT THE SQUARES

void horizontal_vertical_lines( struct Line* lines, int len, struct
        Line* horizon, struct Line* vertical)
{
	//thtea en degree
	int j = 0, k = 0;
	for (int i = 0; i < len; i++)
	{
		float t = lines[i].theta;
		//printf("t value %f \n",t);
		float foundTheta = t *theta;
		if ( foundTheta< 0.01 ||  fabs(foundTheta - M_PI) < 0.01)
		//if ( (t>= -10 && t <= 10) || (t>= 170 && t<=190))

		{
			vertical[j] = lines[i];
			j++;
		}
		else if ( fabs(foundTheta - M_PI/2 ) < 0.01)
		{
			horizon[k] = lines[i];
			k++;
		}
	}
	//printf("len h %i, len v %i \n",j+1,k+1);
}


void sort_horizontal_lines(struct Line* horizon, int len)
{
	int i, j;
	struct Line compare;

	for (i = 1; i < len; i++)
	{
		//printf("(%f,%f)",horizon[i].start.x,horizon[i].start.y);
		compare = horizon[i];
		j = i - 1;

		// Move elements that are greater
                // than key to one position ahead of their current position
		while (j >= 0 && horizon[j].start.y > compare.start.y)
		{
			horizon[j + 1] = horizon[j];
			j = j - 1;
		}
		horizon[j + 1] = compare;
	}

/*	for(int k = 0; k <len; k++)
		printf("(%f,%f) \n",horizon[k].start.x,horizon[k].start.y);
	printf("\n");*/

}

void sort_vertical_lines(struct Line* vertical, int len)
{
	int i, j;
	struct Line compare;

	for (i = 1; i < len; i++)
	{
		compare = vertical[i];
		j = i - 1;

		// Move elements that are greater
                // than key to one position ahead of their current position
		while (j >= 0 && vertical[j].start.x > compare.start.x)
		{
			vertical[j + 1] = vertical[j];
			j = j - 1;
		}
		vertical[j + 1] = compare;
	}

/*	for(int k = 0; k <len; k++)
		printf("(%f,%f)",vertical[k].start.x,vertical[k].start.y);
	printf("\n");*/
}

/*
void fillsquares(struct Line* vertical, struct Line* horizon,
        struct Squares* squares, int len)
{
    int nx = 0;

    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - 1; j++)
        {
            squares[nx].topleft.y = horizon[i].start.y;
            squares[nx].topleft.x = vertical[j].start.x;

            squares[nx].topright.y = horizon[i].start.y;
            squares[nx].topright.x = vertical[j + 1].start.x;

            squares[nx].bottomleft.y = horizon[i + 1].start.y;
	    squares[nx].bottomleft.x = vertical[j].start.x;

	    squares[nx].bottomright.y = horizon[i + 1].start.y;
	    squares[nx].bottomright.x = vertical[j + 1].start.x;
	    nx++;
	}
    }
}*/

void fillsquares(struct Line* vertical, struct Line* horizon, struct Squares* squares, int len) 
{
    int nx = 0;

    // Iterate over each horizontal line
    for (int i = 0; i < len - 1; i++) {
        // For each horizontal line, iterate over each vertical line
        for (int j = 0; j < len - 1; j++) {
            squares[nx].topleft.y = horizon[i].start.y;
            squares[nx].topleft.x = vertical[j].start.x;

            squares[nx].topright.y = horizon[i].start.y;
            squares[nx].topright.x = vertical[j + 1].start.x;

            squares[nx].bottomleft.y = horizon[i + 1].start.y;
            squares[nx].bottomleft.x = vertical[j].start.x;

            squares[nx].bottomright.y = horizon[i + 1].start.y;
            squares[nx].bottomright.x = vertical[j + 1].start.x;
            nx++;
        }
    }
}

// A helper function to compare lines for sorting
int compare_Vertical_Lines(const void *a, const void *b) {
    struct Line *line1 = (struct Line *)a;
    struct Line *line2 = (struct Line *)b;

    // Compare based on x-coordinate for vertical lines 
    return (line1->start.x - line2->start.x); 
}

// A helper function to compare lines for sorting
int compare_Horizon_Lines(const void *a, const void *b) {
    struct Line *line1 = (struct Line *)a;
    struct Line *line2 = (struct Line *)b;

    // y-coordinate for horizontal lines
    return (line1->start.y - line2->start.y);
}

// Function to check if a sequence of lines are evenly spaced
int isEvenlySpacedSequence_vertical(struct Line* lines, int start, int sequenceLength, float averageDistance, float tolerance) {
    for (int i = start; i < start + sequenceLength - 1; i++) {

        float distance = fabs(lines[i + 1].start.x - lines[i].start.x); 
		float minDistance = 0.000001 * averageDistance;
		
 		if (fabs(distance - averageDistance) > tolerance) {
            return 0; // Not evenly spaced
        }
    }
    return 1; // Evenly spaced
}


// Function to check if a sequence of lines are evenly spaced
int isEvenlySpacedSequence_horizon(struct Line* lines, int start, int sequenceLength, float averageDistance, float tolerance) 
{
    for (int i = start; i < start + sequenceLength - 1; i++)
	 {
        float distance = fabs(lines[i + 1].start.y - lines[i].start.y); 
		float minDistance = 0.00000001 * averageDistance;
        if (fabs(distance - averageDistance) > tolerance ) {
            return 0; // Not evenly spaced
        }
    }
    return 1; // Evenly spaced
}

void get_sudoku_lines (struct Line* vertical, struct Line* horizon, struct Line *topLinesv,
struct Line *topLinesh, int len) 
{
	//printf("%f",horizon[1].end.x);
	int topCount = 10;
	int lineCount = len/2;
	//vertical
	// Sort the lines

    qsort(vertical, len/2, sizeof(struct Line), compare_Vertical_Lines);
	
	// We assume the lines are sorted by their x-coordinate (for vertical lines)
    float averageDistance = 0;
    int count = 0;

    // Calculate average distance between lines
    for (int i = 0; i < lineCount - 1; ++i) {
        averageDistance += fabs(vertical[i + 1].start.x - vertical[i].start.x);
        count++;
    }
    if (count > 0) averageDistance /= count;
	printf("averageDistance %f \n",averageDistance);

    float tolerance = averageDistance * 0.2; // 20% tolerance
    int foundCount = 0, i = 0;

    // Look for a sequence of 10 lines with consistent spacing
    while (foundCount < topCount && i <= lineCount - 10) {
        if (isEvenlySpacedSequence_vertical(vertical, i, 10, averageDistance, tolerance)) {
            for (int j = i; j < i + 10 && foundCount < topCount; j++) {
				
                topLinesv[foundCount].start.x = vertical[j].start.x;
				topLinesv[foundCount].start.y = vertical[j].start.y;
				topLinesv[foundCount].end.x = vertical[j].end.x;
				topLinesv[foundCount].end.y = vertical[j].end.y;

				foundCount++;
			printf("vertical1 %d: Start (%f, %f) - End (%f, %f)\n", j,
                                topLinesv[foundCount].start.x, topLinesv[foundCount].start.y,
                                topLinesv[foundCount].end.x, topLinesv[foundCount].end.y);


            }
            break; // Found a sequence, exit loop
        } else {
            i++;
        }
    }
	/*for (int i = 0; i < 10; ++i) {
			printf("topLinesv %d: Start (%f, %f) - End (%f, %f)\n", i,
                                topLinesv[i].start.x, topLinesv[i].start.y,
                                topLinesv[i].end.x, topLinesv[i].end.y);

		}*/

	//horizon
	//vertical
	// Sort the lines
    qsort(horizon, len/2, sizeof(struct Line), compare_Horizon_Lines);

	// We assume the lines are sorted by their x-coordinate (for vertical lines)
    float averageDistancey = 0;
    int county = 0;

    // Calculate average distance between lines
    for (int i = 0; i < lineCount - 1; ++i) {
        averageDistancey += fabs(horizon[i + 1].start.y - horizon[i].start.y);
        county++;
    }
    if (county > 0) averageDistancey /= county;

    float tolerancey = averageDistance * 0.2; // 20% tolerance
    int foundCounty = 0, iy = 0;
	

    // Look for a sequence of 10 lines with consistent spacing
    while (foundCounty < topCount && iy <= lineCount - 10) {
        if (isEvenlySpacedSequence_horizon(horizon, iy, 10, averageDistancey, tolerancey)) {
            for (int j = iy; j < iy + 10 && foundCounty < topCount; j++) {
                topLinesh[foundCounty].start.x = horizon[j].start.x;
				topLinesh[foundCounty].start.y = horizon[j].start.y;
				topLinesh[foundCounty].end.x = horizon[j].end.x;
				topLinesh[foundCounty].end.y = horizon[j].end.y;
				foundCounty++;
            }
            break; // Found a sequence, exit loop
        } else {
            iy++;
        }
    }
}

void print_h_v(struct Line* lines, int len, struct Line* horizon, struct Line* vertical,
struct Line* topLinesh,struct Line* topLinesv)
{
	
	if (!horizon || !vertical)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		free(horizon);
		free(vertical);
		return;
	}

	horizontal_vertical_lines(lines, len, horizon, vertical);



	sort_horizontal_lines(horizon, len /2 );
	

	sort_vertical_lines(vertical, len/2);

	get_sudoku_lines (vertical, horizon, topLinesv, topLinesh, len) ;

	free(horizon);
	free(vertical);

}


void printvalues(struct Line* lines, int len)
{
	struct Line* horizon = calloc( len/2, sizeof(struct Line));
	struct Line* vertical = calloc( len/2, sizeof(struct Line));

	if (!horizon || !vertical)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		free(horizon);
		free(vertical);
		return;
	}

	horizontal_vertical_lines(lines, len, horizon, vertical);

	sort_horizontal_lines(horizon, len /2 );

	sort_vertical_lines(vertical, len/2);

	struct Line* topLinesh = calloc( 10, sizeof(struct Line));
	struct Line* topLinesv = calloc( 10, sizeof(struct Line));

	get_sudoku_lines (vertical, horizon, topLinesv, topLinesh, len) ;


	len = 20;

	int num_squares = (len/2 -1) * (len/2 -1);

	struct Squares* squares = calloc(num_squares, sizeof(struct Squares));

/*	if (!squares) {
		fprintf(stderr, "Memory allocation failed.\n");*/

	fillsquares(topLinesv, topLinesh, squares, 10);


	for (int i = 0; i < num_squares; i++)
	{
		printf("%i \n",i);
		printf("Top Left: (%f, %f)\n",
                        squares[i].topleft.x, squares[i].topleft.y);
		printf("Top Right: (%f, %f)\n",
                        squares[i].topright.x, squares[i].topright.y);
		printf("Bottom Right: (%f, %f)\n",
                        squares[i].bottomright.x, squares[i].bottomright.y);
		printf("Bottom Left: (%f, %f)\n",
                        squares[i].bottomleft.x, squares[i].bottomleft.y);
		printf("\n");

	}
	free(horizon);
	free(vertical);
	free(squares);
}


//Calculate the average  with and height of the  squares
void calculate_average_dimensions(struct Squares* squares, int num_squares, 
float *average_width, float *average_height) {

//int n = 0;
    float total_width = 0, total_height = 0;
    
    for (int i = 0; i < num_squares; i++) 
	{
        float width = squares[i].topright.x - squares[i].topleft.x;
        float height = squares[i].bottomleft.y - squares[i].topleft.y;

		//if (fabs(width - height) <10)
		//{
        total_width += width;
        total_height += height;
	
		//}
    }
    if (num_squares > 0) {
        *average_width = total_width / num_squares;
        *average_height = total_height / num_squares;
    } else {
        *average_width = 0;
        *average_height = 0;
    }
}

//sort squares in order
int compare_squares(const void *a, const void *b) {
    struct Squares *squareA = (struct Squares *)a;
    struct Squares *squareB = (struct Squares *)b;

    // First sort by top Y coordinate, then by left X coordinate
    if (squareA->topleft.y < squareB->topleft.y) return -1;
    if (squareA->topleft.y > squareB->topleft.y) return 1;
    if (squareA->topleft.x < squareB->topleft.x) return -1;
    if (squareA->topleft.x > squareB->topleft.x) return 1;

    return 0;
}

void sort_squares_horizontal(struct Squares* squares, int num_squares) {
    qsort(squares, num_squares, sizeof(struct Squares), compare_squares);
}



struct Squares* drawsquares(struct Line* lines, int len)
{
	struct Line* horizon = calloc( len/2, sizeof(struct Line));
	struct Line* vertical = calloc( len/2, sizeof(struct Line));

	if (!horizon || !vertical)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		free(horizon);
		free(vertical);
	}

	horizontal_vertical_lines(lines, len, horizon, vertical);

	sort_horizontal_lines(horizon, len /2 );

	sort_vertical_lines(vertical, len/2);

	int num_squares = (len/2 -1) * (len/2 -1);

//	struct Squares* sq = calloc(num_squares, sizeof(struct Squares));

	struct Squares* sq = calloc(num_squares, sizeof(struct Squares));

/*	if (!squares) {
		fprintf(stderr, "Memory allocation failed.\n");*/
	fillsquares(vertical, horizon, sq, len/2);


    //sort_squares_horizontal(sq, num_squares);


	return sq;
/*	free(horizon);
	free(vertical);
	free(squares);*/
}






// Function to check if the squares are part of a consistent sequence
int isPartOfSequence(struct Squares* squares, int index, int num_squares, float average_dimension, float tolerance) {
    const int sequenceLength = 10; // Looking for 10 squares in sequence

    if (index + sequenceLength > num_squares) {
        return 0; // Not enough squares left for a full sequence
    }

    for (int i = index; i < index + sequenceLength - 1; i++) {
        float distance = squares[i + 1].topleft.x - squares[i].topleft.x;
        if (fabs(distance - average_dimension) > tolerance) {
            return 0; // Distance is outside of tolerance
        }
    }
    return 1; // All distances within sequence are within tolerance
}

// Find the horizontal and vertical edges of the Sudoku
int findSudokuEdges(struct Squares* squares, int num_squares, float average_dimension) {
    float tolerance = average_dimension * 0.2; // 20% tolerance


    
   // struct Squares horizontalEdges[2]; // Top and bottom horizontal lines
    //struct Squares verticalEdges[2];   // Left and right vertical lines

	int i = 0;
    // Find vertical edges
    while( i < num_squares) {
        if (isPartOfSequence(squares, i, num_squares, average_dimension, tolerance)) {
            //verticalEdges[0] = squares[i]; // Left edge
            //verticalEdges[1] = squares[i + 9]; // Right edge
			return i;
            break;
        }
		i++;
    }
	return 0;
	//int edges_length = verticalEdges[1].topright.x - verticalEdges[1].topleft.x;

 	//horizontalEdges[1] = squares[i+ 72];
}





// Function to extract and save squares as images
void extract_and_save_squares(SDL_Surface* original_image, struct Squares* squares, int num_squares) {

    const int target_width = 28;
    const int target_height = 28;
	int j = 0;

	float average_width, average_height;
    calculate_average_dimensions(squares, num_squares, &average_width, &average_height);
	
	int k = findSudokuEdges(squares, num_squares, average_width);


    for (int i = k; i < k+ 89; i++) 
    {
        float width = squares[i].topright.x - squares[i].topleft.x;
        float height = squares[i].bottomleft.y - squares[i].topleft.y;

		 // Check if the square's dimensions are close to the average
        if ((fabs(width - average_width) < 100 || fabs(height - average_height) < 100 ) && fabs(width - height) < 50) 
		{ 
        // Skip this square as its dimensions are too different
        
        // Create a new surface for each square
        SDL_Surface* square_surface = SDL_CreateRGBSurface(0, width, height, original_image->format->BitsPerPixel,
                                                           original_image->format->Rmask, original_image->format->Gmask,
                                                           original_image->format->Bmask, original_image->format->Amask);
        
        if (square_surface == NULL) {
            fprintf(stderr, "SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
            continue;  // Skip this square and move to the next
        }

        // Define the rectangle to be copied
        SDL_Rect square_rect;
        square_rect.x = squares[i].topleft.x;
        square_rect.y = squares[i].topleft.y;
        square_rect.w = width;
        square_rect.h = height;

        // Blit the square area from the original image to the new surface
        SDL_BlitSurface(original_image, &square_rect, square_surface, NULL);
		
		 // Create a new surface for the resized square
        SDL_Surface* resized_surface = SDL_CreateRGBSurface(0, target_width, target_height, original_image->format->BitsPerPixel,
                                                            original_image->format->Rmask, original_image->format->Gmask,
                                                            original_image->format->Bmask, original_image->format->Amask);
        if (resized_surface == NULL) {
            fprintf(stderr, "SDL_CreateRGBSurface failed for resized surface: %s\n", SDL_GetError());
            SDL_FreeSurface(square_surface);
            continue;
        }

		// Blit the square surface to the resized surface with scaling
        SDL_BlitScaled(square_surface, NULL, resized_surface, NULL); 

        // Save each square as an image
        char filename[64];
        sprintf(filename, "square_%d.bmp", j);
		j++;
        if (SDL_SaveBMP(resized_surface, filename) != 0) {
            fprintf(stderr, "SDL_SaveBMP failed: %s\n", SDL_GetError());
        }

        // Free the square surface after saving
        SDL_FreeSurface(square_surface);
		       // SDL_FreeSurface(resized_surface);
		}
    }
}


