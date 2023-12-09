#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emmintrin.h>
#include <err.h>
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
#include "./image_processing/rotation.h"
// square_detection

#include "./image_processing/hough.h"
#include "./image_processing/detection.h"
// detection

int original_image_width = 0;
int original_image_height = 0;
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
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
  } else if (strcmp(argv[1], "autorot") == 0) {
    struct DetectedLines detected = auto_performHoughTransform(surface);

    /*struct Line* lin = detectedd.lines;
      int num = detectedd.count;

      struct DetectedLines detected = averagearray(lin, num);*/

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

  } else {
    errx(EXIT_FAILURE, "Unsupported filter: %s", argv[1]);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  network *n = import_network("NetworkFinal.save");

  unsigned int a[81];

  for (int i = 0; i < 81; ++i) {
    char path[100];
    snprintf(path, sizeof(path), "./data/tst/secondimg/square_%d.bmp", i);
    double *pixels = malloc(784 * sizeof(double));
    get_tab(path, pixels);
    feed_forward(n, pixels);
    a[i] = read_output(n);
    free(pixels);
  }
  for (int i = 0; i < 81; ++i) {
    if (i % 9 == 0 && i != 0)
      printf("\n");
    printf(" %d ", a[i]);
  }
  printf("\n--------------------------\n");
  for (int i = 0; i < 81; ++i) {
    char path[100];
    snprintf(path, sizeof(path), "./data/tst/firstimg/square_%d.bmp", i);
    double *pixels = malloc(784 * sizeof(double));
    get_tab(path, pixels);
    feed_forward(n, pixels);
    a[i] = read_output(n);
    free(pixels);
  }
  for (int i = 0; i < 81; ++i) {
    if (i % 9 == 0 && i != 0)
      printf("\n");
    printf(" %d ", a[i]);
  }
  free_network(n);

  // SOLVER

  if (argc != 3) {
    usage();
  }
  size_t dim;
  if (strcmp(argv[2], "16") == 0)
    dim = 16;
  else
    dim = 9;
  unsigned int **FinalGrid = allocGrid(dim);
  //    char grid[100];
  //  snprintf(grid, sizeof(grid), "./data/%s", argv[1]);

  // gridReader(dim, FinalGrid, grid);
  gridReader(dim, FinalGrid, argv[1]);
  solve(FinalGrid, 0, 0, dim);
  char *name = argv[1];
  strcat(name, ".result");
  gridWriter(dim, FinalGrid, name);
  freeGrid(FinalGrid, dim);
  printf("generated file: %s\n", name);

  return 0;
}
