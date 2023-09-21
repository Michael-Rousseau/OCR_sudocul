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
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = colored;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    draw(renderer, t);
                break;
            case SDL_KEYDOWN:
                if (t == colored)
                    t = grayscale;
                else
                    t = colored;

                draw(renderer, t);
        }
    }
}

int main(int argc, char** argv)
{   
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 400, 400, SDL_WINDOW_SHOWN         | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window, w, h);
    
    SDL_Texture* ctex = SDL_CreateTextureFromSurface(renderer, surface);
    if (ctex == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    surface_to_grayscale(surface);

    SDL_Texture* gtex = SDL_CreateTextureFromSurface(renderer, surface);
    if (gtex == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_FreeSurface(surface);
    
    event_loop(renderer, ctex, gtex);

    SDL_DestroyTexture(ctex);
    SDL_DestroyTexture(gtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

