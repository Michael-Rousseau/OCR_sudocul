#include "SDL2/SDL_surface.h"
#include "err.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void get_tab(const char *path, double *pixels) {
   SDL_Surface *surface = IMG_Load(path);
  if(surface == NULL)
      errx(1, "OH");
  Uint32 *pixel = surface->pixels;
  if(pixel == NULL)
      errx(1, "OH2");
  int pixelIndex = 0;
  printf("---%d\n", surface->h);
  printf("---%d\n", surface->w);

  for (int y = 0; y < 28; ++y) {
    for (int x = 0; x < 28; ++x) {
      Uint8 r, g, b;
      SDL_GetRGB(pixel[y*28 + x], surface->format, &r, &g, &b);
        Uint8 grayscale = (0.3 * r + 0.59 * g + 0.11 * b);

            if (grayscale > 128)
                pixels[pixelIndex] = 1;
            else
                pixels[pixelIndex] = 0;
      pixelIndex++;
    }
  }
  SDL_FreeSurface(surface);
  IMG_Quit();
  SDL_Quit();
}
