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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
    r = (r >= 255) ? 255 : r;
    g = (g >= 255) ? 255 : g;
    b = (b >= 255) ? 255 : b;

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


float** generate_Kernel(int ksize, float sigma)
{
    float sum = 0.0;

    int center = ksize /2;


    //The kernel matrix is a double pointer
    //we assign a specified amount of memory for an arvray of size ksize of type floats

    float** kernel = (float**)malloc(ksize * sizeof(float*));

    for (int i = 0; i < ksize; i++) {
        kernel[i] = (float*)malloc(ksize * sizeof(float));}

    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {

            //x and y must be coordinates such that 0 is in the center of the kernel
            int mx = x - center;
            int my = y - center;

            kernel[y][x] =  1. / (2 * M_PI * sigma * sigma) * exp(-(mx * mx + my * my) / (2 * sigma * sigma));
            sum += kernel[y][x];
        }
    }

    //Normalizing the kernel -> ensures no undesirable change in the image's brightness (the filter behaves consistently).
    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            kernel[y][x] /= sum;
        }
    }
    return kernel;
}




void applyblur (SDL_Surface * image, float** kernel, int kernelsize, SDL_Surface* filteredimage)
{
    int imageWidth = image->w;
    int imageHeight = image->h;

    //filteredimage =  a separate output image, with the same dimensions as the original image

    int center = kernelsize /2;


    //iterating through each pixel in the input image
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {

            float finalr = 0.0;
            float finalb = 0.0;
            float finalg = 0.0;

            //For each pixel, we multiply the pixel values in the image with the corresponding values in the kernel and accumulates the results.
            for (int ky = 0; ky < kernelsize; ky++) {
                for (int kx = 0; kx < kernelsize; kx++) {



                    int Xf = x + kx - center;
                    int Yf = y + ky - center;

                    //check if we are outside of the array
                    if (Xf >= 0 && Xf < imageWidth && Yf < imageHeight && Xf >=0) {
                        Uint32* pixelinitial = (Uint32*)image->pixels;


                        Uint32 pi = pixelinitial[Yf * imageWidth + Xf];

                        Uint8 r,g,b,a;
                        SDL_GetRGBA(pi, image->format, &r, &g, &b, &a);

                        finalr += kernel[kx][ky] * r;
                        finalb += kernel[kx][ky] *b;
                        finalg += kernel[kx][ky] *g;
                    }
                    //if we are outside of the image boundaries, the values are treated as being zero.
                }
            }
            Uint32 newPixelValue = SDL_MapRGBA(filteredimage->format, (Uint8)finalr, (Uint8)finalg, (Uint8)finalb, 255);
            ((Uint32*)filteredimage->pixels)[y * imageWidth + x] = newPixelValue;


        }
    }
}





void surface_to_reducenoise(SDL_Surface* surface)
{
    // Define the kernel size and sigma for Gaussian blur
    int kernelSize = 5;
    float sigma = 4.0;

    // Generate the Gaussian kernel
    float** kernel = generate_Kernel(kernelSize, sigma);

    SDL_Surface* outputimage = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);
    if (outputimage == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    applyblur (surface, kernel, kernelSize, outputimage);

    SDL_BlitSurface(outputimage, NULL, surface, NULL);

    SDL_FreeSurface(outputimage);


    //IMG_SavePNG(outputImage, "output_image.png");

    // Clean up
    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);
}
