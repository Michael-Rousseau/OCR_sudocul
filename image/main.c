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

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
    if (render != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop_grayscale(SDL_Renderer* renderer, SDL_Texture* grayscale)
{
    SDL_Event event;

    draw(renderer, grayscale);
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED){
			draw(renderer, grayscale);
			break;
		}
        }
    }
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// blackwhite: Texture that contains the black and white image.
void event_loop_blackwhite(SDL_Renderer* renderer, SDL_Texture* blackwhite)
{
    SDL_Event event;

    draw(renderer, blackwhite);
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
                                draw(renderer, blackwhite);
                                break;
                        }
        }
    }
}




 
void event_loop_contrast(SDL_Renderer* renderer, SDL_Texture* contrast)
{
    SDL_Event event;

    draw(renderer, contrast);
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
                                draw(renderer, contrast);
                                break;
                        }
        }
    }
}



/* 
void event_loop_"FILTER NAME"(SDL_Renderer* renderer, SDL_Texture* "FILTER NAME")
{
    SDL_Event event;

    draw(renderer, "FILTER NAME");
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
                                draw(renderer, "FILTER NAME");
                                break;
                        }
        }
    }
}

*/


int main(int argc, char** argv)
{   
    // Checks the number of arguments.
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
    
    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window, w, h);
    

    if (strcmp(argv[1], "grayscale") == 0)
    {
            surface_to_grayscale(surface);

            SDL_Texture* grayscale_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (grayscale_texture == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());

            SDL_FreeSurface(surface);
            event_loop_grayscale(renderer, grayscale_texture);
            SDL_DestroyTexture(grayscale_texture);

    } else if (strcmp(argv[1], "blackwhite") == 0){

            surface_to_blackwhite(surface);
            SDL_Texture* blackwhite_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (blackwhite_texture == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());

            SDL_FreeSurface(surface);
            event_loop_blackwhite(renderer, blackwhite_texture);
            SDL_DestroyTexture(blackwhite_texture);

    } else if (strcmp(argv[1], "bright") == 0)
	    // In the surface_to_contrast function, I have a surface and a contrast param.
	    // If the contrast is > 0, we have an increasing contrast
	    // If the contrast is < 0, we have a decreasing contrast
    {
	    surface_to_contrast(surface, 2);

	    SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer, surface);
	    if (contrast_texture == NULL)
		    errx(EXIT_FAILURE, "%s", SDL_GetError());

	    SDL_FreeSurface(surface);
	    event_loop_blackwhite(renderer, contrast_texture);
	    SDL_DestroyTexture(contrast_texture);

    } else if (strcmp(argv[1], "dark") == 0){
	    surface_to_contrast(surface, 0.1);

	    SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer, surface);
	    if (contrast_texture == NULL)
		    errx(EXIT_FAILURE, "%s", SDL_GetError());

	    SDL_FreeSurface(surface);
	    event_loop_blackwhite(renderer, contrast_texture);
	    SDL_DestroyTexture(contrast_texture);

    }/* PLEASE LEAVE THE EXAMPLE !
	 if (strcmp(argv[1], "FILTER NAME") == 0){
		surface_to_"FILTER NAME"(surface);
		SDL_Texture* "FILTER NAME"_texture = SDL_CreateTextureFromSurface(renderer, surface);
		if ("FILTER NAME"_texture == NULL)
                	errx(EXIT_FAILURE, "%s", SDL_GetError());

            	SDL_FreeSurface(surface);
            	event_loop_blackwhite(renderer, "FILTER NAME"_texture);
            	SDL_DestroyTexture("FILTER NAME"_texture);

	 }
	 */
    else {
        //ERROR
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
