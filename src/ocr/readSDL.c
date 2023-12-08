#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void get_tab(const char *path, double *pixels) {
  SDL_Surface *surface = IMG_Load(path);
  Uint32 *pixel = surface->pixels;
  printf("size: %d %d\n", surface->h, surface->w);
  int pixelIndex = 0;
  for (int y = 0; y < surface->h -1; ++y) {
    for (int x = 0; x < surface->w-1; ++x) {
      Uint8 r, g, b;
      SDL_GetRGB(pixel[y * surface->h + x], surface->format, &r, &g, &b);
      Uint8 grayscale = (0.3 * r + 0.59 * g + 0.11 * b);
      if (grayscale < 128)
        pixel[pixelIndex] = 1;
      else
        pixel[pixelIndex] = 0;
      pixelIndex++;
    }
  }
  SDL_FreeSurface(surface);
  IMG_Quit();
  SDL_Quit();
}
