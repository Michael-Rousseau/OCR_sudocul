#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emmintrin.h>
#include <err.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include all main here
#include "./ocr/helper.h"
#include "./ocr/network.h"
#include "./ocr/ocr.h"
#include "./ocr/readSDL.h"
#include "./ocr/thread_pool.h"
// training

#include "./backtracking/allocator.h"
#include "./backtracking/filestream.h"
#include "./backtracking/solver.h"
#include "./backtracking/usage.h"
// main

#include "./image_processing/image.h"
#include "./image_processing/processing.h"
// square_detection

#include "./image_processing/detection.h"
#include "./image_processing/hough.h"
// detection

//So here is an example of how to call functions on the main.
int main(int argc, char **argv) {
  // processing
  if (argc < 3)
    errx(EXIT_FAILURE, "Usage: image-file");

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

  SDL_Surface *surface = load_image(argv[2]);
  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  int w = surface->w;
  int h = surface->h;
  SDL_SetWindowSize(window, w, h);

  if (strcmp(argv[1], "all") == 0) {
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

    //    SDL_Surface *canny = IMG_Load("canny.jpg");
    //   if (!extraction_surface) {
    //   errx(EXIT_FAILURE, "Unable to load image: %s", IMG_GetError());
    //}
    struct DetectedLines detected = performHoughTransform(surface);
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
        findbestsquare(surface, vertical, horizon, num_lines / 2);

    SDL_Texture *grayscale_texture =
        SDL_CreateTextureFromSurface(renderer, surface);
    if (grayscale_texture == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Moved SDL_FreeSurface(surface); to after the squares are extracted

    event_loop_image_test_sq(renderer, grayscale_texture, sq,
                             (num_lines / 2 - 1) * (num_lines / 2 - 1), s);
    extract_and_save_squares(extraction_surface, sq,
                             (num_lines / 2 - 1) * (num_lines / 2 - 1), s);

    free(lines);
    free(lin);
    free(horizon);
    free(vertical);
    free(sq);
    SDL_DestroyTexture(grayscale_texture);
    SDL_FreeSurface(extraction_surface);
    SDL_FreeSurface(surface); // Now freeing the surface after extraction

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    network *n = import_network("Final.nw");

    unsigned int a[81];

    for (int i = 0; i < 81; ++i) {
      char path[100];
      snprintf(path, sizeof(path), "square_%d.bmp", i);
      double *pixels = malloc(784 * sizeof(double));
      get_tab(path, pixels);
      feed_forward(n, pixels);
      a[i] = read_output(n);
      free(pixels);
    }
    //    char* finalString = malloc(100);
    // snprintf(finalString, 100,"./data/grid_0%d\n", 1);//TODO add the name
    FILE *f = fopen("./data/grid_01", "w");
    for (size_t index_dim = 0; index_dim < 9; index_dim++) {
      if (index_dim % 3 == 0 && index_dim != 0)
        fprintf(f, "\n");
      for (size_t j = 0; j < 9; ++j) {
        int c = a[index_dim * 9 + j];
        if (j % 3 == 0 && j != 0) {
          if (c <= 9 && c > 0)
            fprintf(f, " %d", a[index_dim * 9 + j]);
          else
            fprintf(f, " %c", '.');
        } else if (j % 8 == 0 && j != 0) {
          if (c <= 9 && c > 0)
            fprintf(f, "%d\n", a[index_dim * 9 + j]);
          else
            fprintf(f, "%c\n", '.');
        } else {

          if (c <= 9 && c > 0)
            fprintf(f, "%d", a[index_dim * 9 + j]);
          else
            fprintf(f, "%c", '.');
        }
      }
    }
    fclose(f);
    free_network(n);

    // SOLVER

    size_t dim = 9;
    unsigned int **FinalGrid = allocGrid(dim);

    char *r = "./data/grid_01";
    gridReader(dim, FinalGrid, r);
    solve(FinalGrid, 0, 0, dim);
    char *res;
    // asprintf(&res, "%s%s", finalString, ".result");

    // free(finalString);

    gridWriter(dim, FinalGrid, "./data/grid_01.result");
    freeGrid(FinalGrid, dim);
    printf("generated result grid in data");
    //  free(res);
    // So here, all the code will split the grid, load the OCR on it and then
    // write it in a file
  } else if (strcmp(argv[1], "autorot") == 0) {
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

    SDL_Surface *surface = load_image(argv[2]);
    if (surface == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window, w, h);

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
    struct DetectedLines detected = auto_performHoughTransform(surface);

    double angle = calculate_angle(detected);

    SDL_Surface *im = RotateImage(surface, angle);
    IMG_SaveJPG(im, "autorot.jpg", 100);
    struct Line* l = detected.lines;


    SDL_FreeSurface(surface);
    SDL_FreeSurface(im);

    free(l);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

return 0;
}
