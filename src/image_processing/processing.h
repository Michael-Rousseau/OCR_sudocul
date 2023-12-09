#pragma once

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
#include "rotation.h"

void draw(SDL_Renderer* renderer, SDL_Texture* texture);
void event_loop_image(SDL_Renderer* renderer, SDL_Texture* t_image);
void print_accumulator(int** accumulator, int accumulator_height,
        int accumulator_width);
