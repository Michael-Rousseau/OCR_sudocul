
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


void surface_to_inverse(SDL_Surface* surface)
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
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (r == 255)
            r =0;
        else{
            r = 255;
        }
        if (g == 255)
            g =0;
        else{
            g = 255;
        }
        if (b == 255)
            b = 0;
        else{
            b = 255;
        }

        pixels[i] = SDL_MapRGB(format, r, g, b);

    }

    SDL_UnlockSurface(surface);
}





Uint32* integral_image(SDL_Surface* surface) {
    Uint32* integral = malloc(surface->w * surface->h * sizeof(Uint32));
    if (!integral) {
        errx(EXIT_FAILURE, "Memory allocation for integral image failed");
    }

    SDL_LockSurface(surface);

    for (int y = 0; y < surface->h; y++) {
        Uint32 sum = 0;
        for (int x = 0; x < surface->w; x++) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            Uint8 average = (r + g + b) / 3;

            sum += average;
            if (y == 0) {
                integral[y * surface->w + x] = sum;
            } else {
                integral[y * surface->w + x] = integral[(y - 1) *
                    surface->w + x] + sum;
            }
        }
    }

    SDL_UnlockSurface(surface);
    return integral;
}

Uint8 fast_local_threshold(Uint32* integral, int x, int y,
        int neighborhood_size, int width, int height) {
    int side = neighborhood_size / 2;
    int x1 = x - side - 1;
    int y1 = y - side - 1;
    int x2 = x + side;
    int y2 = y + side;

    x1 = (x1 < 0) ? 0 : x1;
    y1 = (y1 < 0) ? 0 : y1;
    x2 = (x2 >= width) ? width - 1 : x2;
    y2 = (y2 >= height) ? height - 1 : y2;

    int count = (x2 - x1) * (y2 - y1);

    Uint32 sum = integral[y2 * width + x2] - integral[y1 * width + x2] -
        integral[y2 * width + x1] + integral[y1 * width + x1];
    return (Uint8)(sum / count);
}



void surface_to_blackwhite(SDL_Surface* surface) {
    int neighborhood_size = 15;
    if (neighborhood_size % 2 == 0) neighborhood_size++;

    SDL_Surface* copy = SDL_ConvertSurfaceFormat(surface,
            SDL_PIXELFORMAT_RGB888, 0);
    if (!copy) {
        errx(EXIT_FAILURE, "Unable to create surface copy: %s", SDL_GetError());
    }

    Uint32* integral = integral_image(copy);

    SDL_LockSurface(surface);
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint8 local_threshold = fast_local_threshold(integral, x, y,
                    neighborhood_size, copy->w, copy->h);

            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            Uint8 average = (r + g + b) / 3;

            if (average < local_threshold) {
                ((Uint32*)surface->pixels)[y * surface->w + x] =
                    SDL_MapRGB(surface->format, 0, 0, 0);
            } else {
                ((Uint32*)surface->pixels)[y * surface->w + x] =
                    SDL_MapRGB(surface->format, 255, 255, 255);
            }
        }
    }

    SDL_UnlockSurface(surface);

    SDL_FreeSurface(copy);
    free(integral);
}



// Adjusts the contrast of a single pixel.
//
// pixel_color: Color of the pixel to adjust in the RGB format.
// format: Format of the pixel used by the surface.
// contrast: Contrast adjustment factor (e.g., 1.5 for increasing contrast).
Uint32 pixel_to_contrast(Uint32 pixel_color, SDL_PixelFormat* format,
        float contrast)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    // Here, we shift the range from [0, 255] to [-128, 127]
    // before adjusting contrast
    float shifted_contrast_scale = (contrast - 1.0) * 128.0;

    // Adjust contrast
    int new_r = r + (int)((r - 128) * shifted_contrast_scale / 128.0);
    int new_g = g + (int)((g - 128) * shifted_contrast_scale / 128.0);
    int new_b = b + (int)((b - 128) * shifted_contrast_scale / 128.0);

    // Clamp the values to the [0, 255] range
    new_r = new_r > 255 ? 255 : new_r < 0 ? 0 : new_r;
    new_g = new_g > 255 ? 255 : new_g < 0 ? 0 : new_g;
    new_b = new_b > 255 ? 255 : new_b < 0 ? 0 : new_b;

    // Return the new color
    return SDL_MapRGB(format, new_r, new_g, new_b);

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

    float** kernel = (float**)malloc(ksize * sizeof(float*));

    for (int i = 0; i < ksize; i++) {
        kernel[i] = (float*)malloc(ksize * sizeof(float));}

    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {

            int mx = x - center;
            int my = y - center;
            double tmp = -(mx * mx + my * my) / (2 * sigma * sigma);

            kernel[y][x] =  1. / (2 * M_PI * sigma * sigma) * exp(tmp);
            sum += kernel[y][x];
        }
    }

    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            kernel[y][x] /= sum;
        }
    }
    return kernel;
}




void applyblur (SDL_Surface * image, float** kernel,
        int kernelsize, SDL_Surface* filteredimage)
{
    int imageWidth = image->w;
    int imageHeight = image->h;
    int center = kernelsize /2;


    //iterating through each pixel in the input image
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {

            float finalr = 0.0;
            float finalb = 0.0;
            float finalg = 0.0;

            for (int ky = 0; ky < kernelsize; ky++) {
                for (int kx = 0; kx < kernelsize; kx++) {



                    int Xf = x + kx - center;
                    int Yf = y + ky - center;

                    //check if we are outside of the array
                    if (Xf >= 0 && Xf < imageWidth && Yf < imageHeight
                            && Xf >=0) {
                        Uint32* pixelinitial = (Uint32*)image->pixels;


                        Uint32 pi = pixelinitial[Yf * imageWidth + Xf];

                        Uint8 r,g,b,a;
                        SDL_GetRGBA(pi, image->format, &r, &g, &b, &a);

                        finalr += kernel[kx][ky] * r;
                        finalb += kernel[kx][ky] *b;
                        finalg += kernel[kx][ky] *g;
                    }
                    //if we are outside of the image boundaries,
                    //the values are treated as being zero.
                }
            }
            Uint32 newPixelValue = SDL_MapRGBA(filteredimage->format,
                    (Uint8)finalr, (Uint8)finalg, (Uint8)finalb, 255);
            ((Uint32*)filteredimage->pixels)[y * imageWidth + x] =
                newPixelValue;


        }
    }
}





void surface_to_reducenoise(SDL_Surface* surface)
{
    // Define the kernel size and sigma for Gaussian blur
    int kernelSize = 9;
    float sigma = 10.0;

    // Generate the Gaussian kernel
    float** kernel = generate_Kernel(kernelSize, sigma);

    SDL_Surface* outputimage = SDL_CreateRGBSurface(0,
            surface->w, surface->h, 32, 0, 0, 0, 0);
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

