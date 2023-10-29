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
#include "hough1.h"

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
    if (render != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

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
                    draw(renderer, t_image);
                    break;
                }
        }
    }
}


void print_accumulator(int** accumulator, int accumulator_height, int accumulator_width) {
    for (int i = 0; i < accumulator_height; i++) {
        for (int j = 0; j < accumulator_width; j++) {
            printf("%d ", accumulator[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char** argv)
{
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


    if (strcmp(argv[1], "grayscale") == 0) {

        surface_to_grayscale(surface);
        SDL_Texture* grayscale_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (grayscale_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, grayscale_texture);
        SDL_DestroyTexture(grayscale_texture);


    } else if (strcmp(argv[1], "blackwhite") == 0) {

        surface_to_blackwhite(surface);
        SDL_Texture* blackwhite_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (blackwhite_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, blackwhite_texture);
        SDL_DestroyTexture(blackwhite_texture);


    } else if (strcmp(argv[1], "bright") == 0) {

        surface_to_contrast(surface, 2);
        SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (contrast_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, contrast_texture);
        SDL_DestroyTexture(contrast_texture);


    } else if (strcmp(argv[1], "dark") == 0) {

        surface_to_contrast(surface, 0.1);
        SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (contrast_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, contrast_texture);
        SDL_DestroyTexture(contrast_texture);


    } else if (strcmp(argv[1], "reducenoise") ==0) {

        surface_to_reducenoise(surface);
        SDL_Texture* blur_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (blur_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, blur_texture);
        SDL_DestroyTexture(blur_texture);


    } else if (strcmp(argv[1], "inverse") == 0) {
        surface_to_inverse(surface);
        SDL_Texture* inverse_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (inverse_texture == NULL){
            errx(EXIT_FAILURE, "%s", SDL_GetError());
        }
        SDL_FreeSurface(surface);
        event_loop_image(renderer, inverse_texture);
        SDL_DestroyTexture(inverse_texture);

    }


    else if (strcmp(argv[1],"process") == 0) {

        surface_to_grayscale(surface);
        IMG_SaveJPG(surface, "grayscale.jpg", 100);

        surface_to_contrast(surface, 0.1);
        IMG_SaveJPG(surface, "contrast.jpg", 100);
        surface_to_reducenoise(surface);
        IMG_SaveJPG(surface, "reducenoise.jpg", 100);

        surface_to_blackwhite(surface);
        IMG_SaveJPG(surface, "t.jpg", 100);

        surface_to_inverse(surface);
        IMG_SaveJPG(surface, "inverse.jpg", 100);

        SDL_Texture* process_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (process_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, process_texture);
        SDL_DestroyTexture(process_texture);


    } else if (strcmp(argv[1], "hough") == 0) {

        int image_height = 10;
        int image_width = 10;
        int** edge_image = malloc(image_height * sizeof(int*));
        for (int i = 0; i < image_height; i++) {
            edge_image[i] = malloc(image_width * sizeof(int));
            for (int j = 0; j < image_width; j++) {
                edge_image[i][j] = (i == j) ? 1 : 0;
            }
        }

        int accumulator_height;
        int accumulator_width;

        int** accumulator = hough_transform(edge_image, image_height, image_width, &accumulator_height, &accumulator_width);

        printf("Accumulator:\n");
        print_accumulator(accumulator, accumulator_height, accumulator_width);

        for (int i = 0; i < image_height; i++) {
            free(edge_image[i]);
        }
        free(edge_image);
        free_accumulator(accumulator, accumulator_height);

    }



    /* PLEASE LEAVE THE EXAMPLE !
       if (strcmp(argv[1], "FILTER NAME") == 0){
       surface_to_"FILTER NAME"(surface);
       SDL_Texture* "FILTER NAME"_texture = SDL_CreateTextureFromSurface(renderer, surface);
       if ("FILTER NAME"_texture == NULL)
       errx(EXIT_FAILURE, "%s", SDL_GetError());

       SDL_FreeSurface(surface);
       event_loop_image(renderer, "FILTER NAME"_texture);
       SDL_DestroyTexture("FILTER NAME"_texture);

       }
       */
       else {
           errx(EXIT_FAILURE, "Unsupported filter: %s", argv[1]);
       }

       SDL_DestroyRenderer(renderer);
       SDL_DestroyWindow(window);
       SDL_Quit();
       return EXIT_SUCCESS;

}
