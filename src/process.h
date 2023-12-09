#pragma once

#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./image_processing/image.h"
#include "./image_processing/processing.h"
// square_detection

#include "./image_processing/hough.h"
#include "./image_processing/detection.h"

void process_all(SDL_Renderer *renderer, SDL_Surface *surface) {
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

    dilation(surface);
    IMG_SaveJPG(surface, "dilation.jpg", 100);

    erosion(surface);
    IMG_SaveJPG(surface, "erosion.jpg", 100);

    Canny_edge_result(surface);
    IMG_SaveJPG(surface, "canny.jpg", 100);

    SDL_Surface *extraction_surface = IMG_Load("erosion.jpg");
    if (!extraction_surface) {
      errx(EXIT_FAILURE, "Unable to load image: %s", IMG_GetError());
    }

    SDL_Surface *canny = IMG_Load("canny.jpg");
    if (!extraction_surface) {
      errx(EXIT_FAILURE, "Unable to load image: %s", IMG_GetError());
    }
    struct DetectedLines detected = performHoughTransform(canny);
    struct Line *lin = detected.lines;
    int num = detected.count;

    struct DetectedLines d2 = averagearray(lin, num);
    struct Line *lines = d2.lines;
    int num_lines = d2.count;

    struct Line *horizon = calloc(num_lines / 2, sizeof(struct Line));
    struct Line *vertical = calloc(num_lines / 2, sizeof(struct Line));

    if (!horizon || !vertical) {
      fprintf(stderr, "Memory allocation failed.\n");
      free(horizon);
      free(vertical);
    }

    struct Squares *sq = drawsquares(lines, num_lines, horizon, vertical);
    struct Squares s =
        findbestsquare(canny, vertical, horizon, num_lines / 2);

    SDL_Texture *grayscale_texture =
        SDL_CreateTextureFromSurface(renderer, canny);
    if (grayscale_texture == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Moved SDL_FreeSurface(surface); to after the squares are extracted

    event_loop_image_test_sq(renderer, grayscale_texture, sq,
                             (num_lines / 2 - 1) * (num_lines / 2 - 1), s);
    SDL_DestroyTexture(grayscale_texture);

    extract_and_save_squares(extraction_surface, sq,
                             (num_lines / 2 - 1) * (num_lines / 2 - 1), s);

    SDL_FreeSurface(canny); // Now freeing the surface after extraction

    free(lines);
    free(horizon);
    free(vertical);
    free(sq);
    SDL_FreeSurface(extraction_surface);
}

void process_autorot(SDL_Renderer *renderer, SDL_Surface *surface) {
    struct DetectedLines detected = auto_performHoughTransform(surface);

    double angle = calculate_angle(detected);

    SDL_Surface *im = RotateImage(surface, angle);
    IMG_SaveJPG(im, "autorot.jpg", 100);

    SDL_Texture *im_txt = SDL_CreateTextureFromSurface(renderer, im);
    if (im_txt == NULL) {
      errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    SDL_FreeSurface(im);

    SDL_DestroyTexture(im_txt);
    free(detected.lines);
}

void process_image(char *path) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Window *window =
      SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 400, 400,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Surface *surface = load_image(path);
  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  int w = surface->w;
  int h = surface->h;
  SDL_SetWindowSize(window, w, h);

  process_all(renderer, surface);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
