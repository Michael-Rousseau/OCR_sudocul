/*
 * =====================================================================================
 *
 *       Filename:  image.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/09/2023 15:18:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */

#ifndef IMAGE_H
#define IMAGE_H

void surface_to_grayscale(SDL_Surface* surface);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
SDL_Surface* load_image(const char* path);
void surface_to_blackwhite(SDL_Surface* surface);
Uint32 pixel_to_blackwhite(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_contrast(SDL_Surface* surface, float contrast);
Uint32 pixel_to_contrast(Uint32 pixel_color, SDL_PixelFormat* format, float contrast);
float** generate_Kernel(int ksize, float sigma);
void applyblur (SDL_Surface * image, float** kernel, int kernelsize, SDL_Surface* filteredimage);
void surface_to_reducenoise(SDL_Surface* surface);

#endif
