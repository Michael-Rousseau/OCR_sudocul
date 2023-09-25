/*
 * =====================================================================================
 *
 *       Filename:  image.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/09/2023 15:07:09
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

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* is = IMG_Load(path);
    if (is == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* fs = SDL_ConvertSurfaceFormat(is, SDL_PIXELFORMAT_RGB888, 0);
    if (fs == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(is);
    return fs;
}

// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    double average = 0.3*r + 0.59*g + 0.11*b;
    return SDL_MapRGB(format, average, average, average);
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if (format == NULL || pixels == NULL || SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < len; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    }

    SDL_UnlockSurface(surface);
}



// Converts a colored pixel into black and white.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_blackwhite(Uint32 pixel_color, SDL_PixelFormat* format)
{
        Uint8 r, g, b;
        SDL_GetRGB(pixel_color, format, &r, &g, &b);

        int threshold = 164;

        Uint8 average = 0.3*r + 0.59*g + 0.11*b;
        if (average > threshold)
        {
                r = 255;
                g = 255;
                b = 255;
        } else
        {
                r = 0;
                g = 0;
                b = 0;

        }

        Uint32 color = SDL_MapRGB(format, r, g, b);
        return color;
}

void surface_to_blackwhite(SDL_Surface* surface)
{
        Uint32* pixels = surface->pixels;
        if (pixels == NULL)
                errx(EXIT_FAILURE, "%s", SDL_GetError());

        int len = surface->w * surface->h;

        SDL_PixelFormat* format = surface->format;
        if (format == NULL)
                errx(EXIT_FAILURE, "%s", SDL_GetError());

        int i=0;
        SDL_LockSurface(surface);

        while (i < len){
                pixels[i] = pixel_to_blackwhite(pixels[i], format);
                i++;
        }

        SDL_UnlockSurface(surface);

}



// Adjusts the contrast of a single pixel.
//
// pixel_color: Color of the pixel to adjust in the RGB format.
// format: Format of the pixel used by the surface.
// contrast: Contrast adjustment factor (e.g., 1.5 for increasing contrast).
Uint32 pixel_to_contrast(Uint32 pixel_color, SDL_PixelFormat* format, float contrast)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    // Adjust the contrast of each channel individually
    r = (Uint8)(r * contrast);
    g = (Uint8)(g * contrast);
    b = (Uint8)(b * contrast);

    // Ensure values stay within the valid range (0-255)
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;

    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}


// Adjusts the contrast of an SDL surface.
//
// surface: The SDL surface to adjust.
// contrast: Contrast adjustment factor (e.g., 1.5 for increasing contrast).
void surface_to_contrast(SDL_Surface* surface, float contrast)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int i = 0;
    SDL_LockSurface(surface);

    while (i < len) {
        pixels[i] = pixel_to_contrast(pixels[i], format, contrast);
        i++;
    }

    SDL_UnlockSurface(surface);
}

