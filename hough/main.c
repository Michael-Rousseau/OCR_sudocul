/*
 * =====================================================================================
 *
 *       Filename:  renderer.c
 *
 *    Description: Load and preprocess images 
 *
 *        Version:  1.0
 *        Created:  20/09/2023 15:03:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
//#include "hough1.h"
#include "joanneh.h"
//#include "detection.h"

int original_image_width = 0;
int original_image_height = 0;

void update_render_scale(SDL_Renderer* renderer, int new_width, int new_height) 
{
    float scale_x = (float)new_width / original_image_width;
    float scale_y = (float)new_height / original_image_height;
    SDL_RenderSetScale(renderer, scale_x, scale_y);
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
	int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (render != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_RenderPresent(renderer);
}



void drawk(SDL_Renderer* renderer, SDL_Texture* texture, struct Line* lines,int num_lines)
{
	int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (render != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//	SDL_RenderClear(renderer);


	// Draw detected lines
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for lines
	for (int i = 0; i < num_lines; i++) {
		SDL_RenderDrawLine(renderer, lines[i].start.x, lines[i].start.y, lines[i].end.x, lines[i].end.y);
	}
	// SDL_RenderPresent(renderer);

	SDL_RenderPresent(renderer);
}


void draw_squares(SDL_Renderer* renderer, SDL_Texture* texture, struct Squares* squares ,int num_lines)
{
	int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (render != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//	SDL_RenderClear(renderer);


	// Draw detected lines
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for lines
	for (int i = 0; i < num_lines; i++) {
	//	SDL_RenderDrawLine(renderer, 100,500, 500,500);
		printf("draw squares start x %f, y %f \n", squares[i].topleft.x, squares[i].topleft.y);

		SDL_RenderDrawLine(renderer, squares[i].topleft.x, squares[i].topleft.y, squares[i].topleft.x +1, squares[i].topleft.y+1 );
		SDL_RenderDrawLine(renderer, squares[i].topright.x, squares[i].topright.y,squares[i].topright.x +1, squares[i].topright.y+1);
		SDL_RenderDrawLine(renderer, squares[i].bottomright.x, squares[i].bottomright.y, squares[i].bottomright.x+1, squares[i].bottomright.y+1);

		SDL_RenderDrawLine(renderer,squares[i].bottomleft.x, squares[i].bottomleft.y,squares[i].bottomleft.x+1, squares[i].bottomleft.y +1);


	}
	 //SDL_RenderPresent(renderer);

	SDL_RenderPresent(renderer);
}



void event_loop_image(SDL_Renderer* renderer, SDL_Texture* t_image)
{
	SDL_Event event;

	draw(renderer, t_image);
	while (1)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
				return;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					//update_render_scale(renderer, event.window.data1, event.window.data2);

					draw(renderer, t_image);
					break;
				}
		}
	}
}





void event_loop_image_l(SDL_Renderer* renderer, SDL_Texture* t_image, struct Line* line, int n)
{
	SDL_Event event;

	draw(renderer, t_image);
	while (1)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
				return;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					update_render_scale(renderer, event.window.data1, event.window.data2);

					drawk(renderer, t_image,line, n);
					break;
				}
		}
	}
}
void event_loop_image_test_averagelines(SDL_Renderer* renderer, SDL_Texture* t_image, struct Line* lines ,int numline) {
    SDL_Event event;

    draw(renderer, t_image);
    while (1) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    //drawl(renderer, t_image, lines);
		    update_render_scale(renderer, event.window.data1, event.window.data2);
		    drawl(lines, numline,renderer, t_image);
                    break;
                }
        }
    }
}

void event_loop_image_test_sq(SDL_Renderer* renderer, SDL_Texture* t_image, struct Squares* squares,int num) {
    SDL_Event event;

    draw(renderer, t_image);
    while (1) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    //drawl(renderer, t_image, lines);
                    	update_render_scale(renderer, event.window.data1, event.window.data2);

			draw_squares(renderer, t_image, squares , num);

                    break;
                }
        }
    }
}





int main(int argc, char **argv){

	if (argc != 3)
		errx(EXIT_FAILURE, "Usage: image-file");

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 400, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Surface* surface = load_image(argv[2]);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	
	original_image_width = surface->w;
	original_image_height = surface->h;

	int image_width = surface->w;
	int image_height = surface->h;
	SDL_SetWindowSize(window, image_width, image_height);


	if (strcmp(argv[1],"hough") ==0)
	{
		//struct Line* lines= performHoughTransform(surface);
		//
		struct DetectedLines detected = performHoughTransform(surface);
		struct Line* lines = detected.lines;
		int num_lines = detected.count;

		for (int i = 0; i < num_lines; ++i) {
			printf("Line %d: Start (%f, %f) - End (%f, %f)\n", i, lines[i].start.x, lines[i].start.y, lines[i].end.x, lines[i].end.y);

		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
			errx(EXIT_FAILURE, "%s", SDL_GetError());
		event_loop_image_l(renderer, texture,lines, num_lines);

		// Cleanup
		free(lines);
	//	free(detected);
		SDL_DestroyTexture(texture);
	}

	else if  (strcmp(argv[1], "detection") == 0) 
	{
			struct DetectedLines detected = performHoughTransform(surface);
			struct Line* lines = detected.lines;
			int num_lines = detected.count;

			SDL_Texture* grayscale_texture = SDL_CreateTextureFromSurface(renderer, surface);
			if (grayscale_texture == NULL)
				errx(EXIT_FAILURE, "%s", SDL_GetError());

			SDL_FreeSurface(surface);
			event_loop_image_test_averagelines(renderer, grayscale_texture, lines, num_lines);
			SDL_DestroyTexture(grayscale_texture);

		//	free(detected);
			free(lines);

		}

	else if (strcmp(argv[1], "square") == 0) {
			struct DetectedLines detected = performHoughTransform(surface);
			struct Line* lines = detected.lines;
			int num_lines = detected.count;

			printvalues(lines,num_lines);



	}
	else if (strcmp(argv[1], "drawsquare") == 0) 
	{
		struct DetectedLines detected = performHoughTransform(surface);
		struct Line* lin = detected.lines;
		int num = detected.count;


		struct DetectedLines d2 = averagearray(lin, num);
		struct Line* lines = d2.lines;
		int num_lines = d2.count;
		printf("num line main %i \n", num_lines);

		for (int i = 0; i < num_lines; i++)
		{
			printf(" i = %i",i);
			printf("%f",lines[i].start.x);
			printf("%f",lines[i].start.y);
			printf("%f",lines[i].end.x);
			printf("%f \n",lines[i].end.y);
		}

			//	printf("n carre main %i", (num_lines/2 -1) * (num_lines/2 -1));

		struct Squares* sq = drawsquares( lines, num_lines );
	//	printf("n carre main %i", (num_lines/2 -1) * (num_lines/2 -1));

		SDL_Texture* grayscale_texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (grayscale_texture == NULL)
			errx(EXIT_FAILURE, "%s", SDL_GetError());

		SDL_FreeSurface(surface);
		event_loop_image_test_sq(renderer, grayscale_texture, sq, (num_lines/2 -1) * (num_lines/2 -1));
		SDL_DestroyTexture(grayscale_texture);

		//	free(detected);
		free(lines);
		free(sq);


	}

		else {
			errx(EXIT_FAILURE, "Unsupported filter: %s", argv[1]);
		}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}







