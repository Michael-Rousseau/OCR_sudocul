/*
 * =====================================================================================
 *
 *       Filename:  renderer.c
 *
 *    Description: Load and preprocess images 
 *
 *        Version:  1.0
 *        Created:  20/09/2023 15:03:34
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
#include "hough1.h"




void draw_line_on_surface(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 color) {

    printf( "drawing\n" );
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        // Check boundaries before drawing the pixel
        if (x0 >= 0 && x0 < surface->w && y0 >= 0 && y0 < surface->h) {
            Uint32 *pixel = (Uint32 *)surface->pixels + y0 * surface->pitch / 4 + x0;
            *pixel = color;
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}




void get_line_endpoints(SDL_Surface *surface, struct point* point, int theta, int image_width, int image_height) {
    // Calculate diagonal (hypotenuse of the image)
    double diagonal = sqrt(image_width * image_width + image_height * image_height);
    double rho_max = diagonal;
    double rho_min = -diagonal;

    // Calculate the end points of the line
    int x1 = point->x + rho_max * -sin(theta);
    int y1 = point->y + rho_max * cos(theta);
    int x2 = point->x + rho_min * -sin(theta);
    int y2 = point->y + rho_min * sin(theta);

    printf("x1 = %d and y1 = %d and x2 = %d and y2 = %d\n", x1, y1, x2, y2 );
    // Define color for line (red in this case)
    Uint32 color = SDL_MapRGBA(surface->format, 255, 0, 0, 255); // ARGB: Red, fully opaque
    // Draw the line on the surface using the provided function
    draw_line_on_surface(surface, x1, y1, x2, y2, color);
}


void find_peaks(SDL_Surface *surface, int** accumulator, int accumulator_height, int accumulator_width, int threshold, int max, int diagonal) {

    for (int r = 0; r < accumulator_height; r++){
        for (int theta = 0; theta < accumulator_width; theta++){
            if (accumulator[r][theta] > threshold && accumulator[r][theta] < max) {
                struct point p;
                p.x = r * cos(theta);
                p.y = r * sin(theta);
                printf("accumulator[r][theta] == %d\n", accumulator[r][theta]);
                printf("Peak was found, %d, %d\n", p.x, p.y);
                get_line_endpoints(surface, &p, theta, accumulator_width, accumulator_height);
            }
        }
    }
}






int** surface_to_edge_image(SDL_Surface *surface) {
    // USED

    int width = surface->w;
    int height = surface->h;
    SDL_LockSurface(surface);

    int **edge_image = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        edge_image[i] = malloc(width * sizeof(int));
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;
            int sum = 0;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    Uint32 pixel = ((Uint32*)surface->pixels)[(y + i) * surface->pitch / 4 + (x + j)];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

                    sumX += r * Gx[i + 1][j + 1];
                    sumY += r * Gy[i + 1][j + 1];
                }
            }

            sum = abs(sumX) + abs(sumY);

            sum = sum > 255 ? 255 : sum;
            sum = sum < 0 ? 0 : sum;

            edge_image[y][x] = sum;
        }
    }

    SDL_UnlockSurface(surface);

    return edge_image;
}





void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
    if (render != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_RenderPresent(renderer);
}


void event_loop_image(SDL_Renderer* renderer, SDL_Texture* t_image)
{
    SDL_Event event;

    draw(renderer, t_image);
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, t_image);
                    break;
                }
        }
    }
}





void print_accumulator(int** accumulator, int accumulator_height, int accumulator_width) {
    for (int i = 0; i < accumulator_height; i++) {
        for (int j = 0; j < accumulator_width; j++) {
            printf("%d ", accumulator[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char **argv){

    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 400, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = load_image(argv[2]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int image_width = surface->w;
    int image_height = surface->h;
    SDL_SetWindowSize(window, image_width, image_height);



    if (strcmp(argv[1], "test") == 0){
        // Convert the image into an **edge_image
        int** edge_image = surface_to_edge_image(surface);

        // Create an accumulator height and width
        int accumulator_height, accumulator_width;

        // Get the accumulator values
        int **accumulator = hough_transform(edge_image, image_height, image_width, &accumulator_height, &accumulator_width);

        if (!accumulator) {
            fprintf(stderr, "Memory allocation failed for accumulator.\n");
            exit(1);
        }


        // Find the maximum value in the accumulator
        int max_value = 0;
        for (int y = 0; y < accumulator_height; y++) {
            for (int x = 0; x < accumulator_width; x++) {
                if (accumulator[y][x] > max_value) {
                    max_value = accumulator[y][x];
                    printf("Max value :%d \n", max_value);
                }
            }
        }
        int diagonal = sqrt(image_height * image_height + image_width * image_width);
        int threshold = max_value * 0.4;

        printf("calling find_peaks\n");
        find_peaks(surface, accumulator, accumulator_width, accumulator_height, threshold, max_value, diagonal);

        printf("called find_peaks\n");


        //Uint32 color = SDL_MapRGBA(surface->format, 255, 0, 0, 255);
        //draw_line_on_surface(surface, 0, 0, image_height, image_width, color);

        printf("max_value = %d", max_value);


        SDL_Surface* accumulator_surface = SDL_CreateRGBSurfaceWithFormat(0, accumulator_width, accumulator_height, 32, SDL_PIXELFORMAT_ARGB8888);

        Uint32 *pixels = (Uint32 *)accumulator_surface->pixels;

        for (int y = 0; y < accumulator_height; y++) {
            for (int x = 0; x < accumulator_width; x++) {
                int normalized_value = 255 * accumulator[y][x] / max_value;

                Uint32 pixel = SDL_MapRGB(accumulator_surface->format, normalized_value, normalized_value, normalized_value);

                int index = y * accumulator_surface->pitch / 4 + x;
                pixels[index] = pixel;
            }
        }



        SDL_SaveBMP(accumulator_surface, "accumulator_visualization.bmp");
        //print_accumulator(accumulator, accumulator_height, accumulator_width);







        SDL_Texture * result_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (result_texture == NULL){
            errx(EXIT_FAILURE, "%s", SDL_GetError());
        }

        event_loop_image(renderer, result_texture);

        IMG_SaveJPG(surface, "result_with_lines.jpg", 100);

        SDL_FreeSurface(accumulator_surface);

        SDL_Texture* test_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (test_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, test_texture);
        SDL_DestroyTexture(test_texture);
    }
    else {
        errx(EXIT_FAILURE, "Unsupported filter: %s", argv[1]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}







