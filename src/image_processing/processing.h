#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "rotation.h"
#include "image.h"
void draw(SDL_Renderer *renderer, SDL_Texture *texture) {
  int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
  if (render != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_RenderPresent(renderer);
}

void draw_test_averagelines(SDL_Renderer *renderer, SDL_Texture *texture,
                            struct Line *lines) {
  int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
  if (render != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  drawl(lines, 26, renderer);

  /*	   for (int i = 0; i <26;i++)
             {

             SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

          // Draw a line
          SDL_RenderDrawLine(renderer, lines[i].start.x ,lines[i].start.y,
          lines[i].end.x, lines[i].end.y);
          }

          */

  SDL_RenderPresent(renderer);
}

void event_loop_image_test_averagelines(SDL_Renderer *renderer,
                                        SDL_Texture *t_image,
                                        struct Line *lines) {
  SDL_Event event;

  draw(renderer, t_image);
  while (1) {
    SDL_WaitEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      return;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        draw_test_averagelines(renderer, t_image, lines);
        // drawl(lines, 6, renderer);
        break;
      }
    }
  }
}

void event_loop_image(SDL_Renderer *renderer, SDL_Texture *t_image) {
  SDL_Event event;

  draw(renderer, t_image);
  while (1) {
    SDL_WaitEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      return;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        draw(renderer, t_image);
        break;
      }
    }
  }
}

void print_accumulator(int **accumulator, int accumulator_height,
                       int accumulator_width) {
  for (int i = 0; i < accumulator_height; i++) {
    for (int j = 0; j < accumulator_width; j++) {
      printf("%d ", accumulator[i][j]);
    }
    printf("\n");
  }
}
