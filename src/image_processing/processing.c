
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
#include "rotation.h"
#include "detection.h"
#include "square_detection.h"

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    int render = SDL_RenderCopy(renderer, texture, NULL, NULL);
    if (render != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());




    SDL_RenderPresent(renderer);
}


void draw_test_averagelines(SDL_Renderer* renderer, SDL_Texture* texture,
        struct Line* lines)
{
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


void event_loop_image_test_averagelines(SDL_Renderer* renderer,
        SDL_Texture* t_image, struct Line* lines) {
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
                    //drawl(lines, 6, renderer);
                    break;
                }
        }
    }
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


void print_accumulator(int** accumulator, int accumulator_height,
        int accumulator_width) {
    for (int i = 0; i < accumulator_height; i++) {
        for (int j = 0; j < accumulator_width; j++) {
            printf("%d ", accumulator[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0,
            400, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = load_image(argv[2]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window, w, h);


    if (strcmp(argv[1], "grayscale") == 0) {

        surface_to_grayscale(surface);
        SDL_Texture* grayscale_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (grayscale_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());


        SDL_FreeSurface(surface);
        event_loop_image(renderer, grayscale_texture);
        SDL_DestroyTexture(grayscale_texture);


    } else if (strcmp(argv[1], "blackwhite") == 0) {

        surface_to_blackwhite(surface);
        SDL_Texture* blackwhite_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (blackwhite_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, blackwhite_texture);
        SDL_DestroyTexture(blackwhite_texture);


    } else if (strcmp(argv[1], "bright") == 0) {

        surface_to_contrast(surface, 2);
        SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (contrast_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, contrast_texture);
        SDL_DestroyTexture(contrast_texture);


    } else if (strcmp(argv[1], "dark") == 0) {

        surface_to_contrast(surface, 0.1);
        SDL_Texture* contrast_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (contrast_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, contrast_texture);
        SDL_DestroyTexture(contrast_texture);


    } else if (strcmp(argv[1], "reducenoise") ==0) {

        surface_to_reducenoise(surface);
        SDL_Texture* blur_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (blur_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(surface);
        event_loop_image(renderer, blur_texture);
        SDL_DestroyTexture(blur_texture);


    } else if (strcmp(argv[1], "inverse") == 0) {
        surface_to_inverse(surface);
        SDL_Texture* inverse_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (inverse_texture == NULL){
            errx(EXIT_FAILURE, "%s", SDL_GetError());
        }
        SDL_FreeSurface(surface);
        event_loop_image(renderer, inverse_texture);
        SDL_DestroyTexture(inverse_texture);

    }


    else if (strcmp(argv[1],"process") == 0) {

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
	IMG_SaveJPG(surface,"dilation.jpg",100);

	erosion(surface);
	IMG_SaveJPG(surface,"erosion.jpg",100);

	Canny_edge_result (surface);
	IMG_SaveJPG(surface,"canny.jpg",100);

//	hough_transform(surface, 10,renderer);
//	IMG_SaveJPG(surface,"hough.jpg",100);


        SDL_Texture* process_texture = SDL_CreateTextureFromSurface(renderer,
                surface);
        if (process_texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());


        SDL_FreeSurface(surface);
        event_loop_image(renderer, process_texture);
        SDL_DestroyTexture(process_texture);


    } /*else if (strcmp(argv[1], "hough") == 0) {

        int image_height = 10;
        int image_width = 10;
        int** edge_image = malloc(image_height * sizeof(int*));
        for (int i = 0; i < image_height; i++) {
            edge_image[i] = malloc(image_width * sizeof(int));
            for (int j = 0; j < image_width; j++) {
                edge_image[i][j] = (i == j) ? 1 : 0;
            }
        }

        int accumulator_height;
        int accumulator_width;

        int** accumulator = hough_transform(edge_image, image_height,
        image_width, &accumulator_height, &accumulator_width);

        printf("Accumulator:\n");
        print_accumulator(accumulator, accumulator_height, accumulator_width);

        for (int i = 0; i < image_height; i++) {
            free(edge_image[i]);
        }
        free(edge_image);
	free_accumulator(accumulator, accumulator_height);

    }*/
 else if (strcmp(argv[1], "rotation") == 0)
    {

	    if (argc > 3) {  // Check if enough arguments are provided
		    double angle = atof(argv[3]);
                    // Convert the argument to a double
		    SDL_Surface* im = RotateImage(surface, angle);
		    IMG_SaveJPG(im,"rot.jpg",100);


		    SDL_Texture* im_txt = SDL_CreateTextureFromSurface(renderer, im);
		    if (im_txt == NULL) {
			    errx(EXIT_FAILURE, "%s", SDL_GetError());
		    }

		    SDL_FreeSurface(surface);
		    SDL_FreeSurface(im);

		    SDL_DestroyTexture(im_txt);
	    }
    }   else if (strcmp(argv[1], "square") == 0) {

	    struct Line lines[16];  // There are 8 horizontal and
                                    // 8 vertical lines
	    int L = 500;
	    // Vertical lines
	    lines[0] = (struct Line){L/9, 0.0, {L/9, 0}, {L/9, L}};
	    lines[1] = (struct Line){2*L/9, 0.0, {2*L/9, 0}, {2*L/9, L}};
	    lines[2] = (struct Line){3*L/9, 0.0, {3*L/9, 0}, {3*L/9, L}};
	    lines[3] = (struct Line){4*L/9, 0.0, {4*L/9, 0}, {4*L/9, L}};
	    lines[4] = (struct Line){5*L/9, 0.0, {5*L/9, 0}, {5*L/9, L}};
	    lines[5] = (struct Line){6*L/9, 0.0, {6*L/9, 0}, {6*L/9, L}};
	    lines[6] = (struct Line){7*L/9, 0.0, {7*L/9, 0}, {7*L/9, L}};
	    lines[7] = (struct Line){8*L/9, 0.0, {8*L/9, 0}, {8*L/9, L}};

	    // Horizontal lines
	    lines[8] = (struct Line){0.0, 5, {0, L/9}, {L, L/9}};
	    lines[9] = (struct Line){0.0, 15, {0, 2*L/9}, {L, 2*L/9}};
	    lines[10] = (struct Line){0.0, 25, {0, 3*L/9}, {L, 3*L/9}};
	    lines[11] = (struct Line){0.0,35, {0, 4*L/9}, {L, 4*L/9}};
	    lines[12] = (struct Line){0.0, 45, {0, 5*L/9}, {L, 5*L/9}};
	    lines[13] = (struct Line){0.0, 55, {0, 6*L/9}, {L, 6*L/9}};
	    lines[14] = (struct Line){0.0, 65, {0, 7*L/9}, {L, 7*L/9}};
	    lines[15] = (struct Line){0.0, 75, {0, 8*L/9}, {L, 8*L/9}};

	    printvalues(lines,16);

    }



    else if (strcmp(argv[1], "detection") == 0) {
	   /* struct Line* lines = (struct Line*)calloc(6, sizeof(struct Line));
	    if (!lines) {
		    fprintf(stderr, "Memory allocation failed.\n");
		    exit(EXIT_FAILURE);
	    }
	    lines[0] = (struct Line){10, 0.0, {10, 0}, {500, 0}};
	    lines[1] = (struct Line){20, 0.0, {20, 0}, {500, 0}};
	    lines[2] = (struct Line){30, 0.0, {30, 0}, {500, 0}};
	    lines[3] = (struct Line){0, 1.5708, {0, 10}, {0, 700}};
	    lines[4] = (struct Line){0, 1.5708, {0, 20}, {0, 700}};
	    lines[5] = (struct Line){0, 1.5708, {0, 30}, {0, 700}};*/


	    struct Line lines[26];  // There are 8 horizontal
                                    // and 8 vertical lines
	    int L = 500;
	    // Vertical lines
	    lines[0] = (struct Line){L/9, 0.0, {L/9, 0}, {L/9, L}};
	    lines[1] = (struct Line){2*L/9, 0.0, {2*L/9, 0}, {2*L/9, L}};
	    lines[2] = (struct Line){3*L/9, 0.0, {3*L/9, 0}, {3*L/9, L}};
	    lines[3] = (struct Line){4*L/9, 0.0, {4*L/9, 0}, {4*L/9, L}};
	    lines[4] = (struct Line){5*L/9, 0.0, {5*L/9, 0}, {5*L/9, L}};
	    lines[5] = (struct Line){6*L/9, 0.0, {6*L/9, 0}, {6*L/9, L}};
	    lines[6] = (struct Line){7*L/9, 0.0, {7*L/9, 0}, {7*L/9, L}};
	    lines[7] = (struct Line){8*L/9, 0.0, {8*L/9, 0}, {8*L/9, L}};

	    // Horizontal lines
	    lines[8] = (struct Line){0.0, 5, {0, L/9}, {L, L/9}};
	    lines[9] = (struct Line){0.0, 15, {0, 2*L/9}, {L, 2*L/9}};
	    lines[10] = (struct Line){0.0, 25, {0, 3*L/9}, {L, 3*L/9}};
	    lines[11] = (struct Line){0.0,35, {0, 4*L/9}, {L, 4*L/9}};
	    lines[12] = (struct Line){0.0, 45, {0, 5*L/9}, {L, 5*L/9}};
	    lines[13] = (struct Line){0.0, 55, {0, 6*L/9}, {L, 6*L/9}};
	    lines[14] = (struct Line){0.0, 65, {0, 7*L/9}, {L, 7*L/9}};
	    lines[15] = (struct Line){0.0, 75, {0, 8*L/9}, {L, 8*L/9}};

	    // Assume the dimension of the Sudoku grid is L x L
	    // Also assume a small deviation factor, say 5 pixels
	    const int deviation = 5;

	    // Noisy lines around the first vertical line
	    lines[16] = (struct Line){L/9 - deviation, 0.0,
                {L/9 - deviation, 0}, {L/9 - deviation, L}};
	    lines[17] = (struct Line){L/9, 0.0, {L/9, 0}, {L/9, L}};
	    lines[18] = (struct Line){L/9 + deviation, 0.0,
                {L/9 + deviation, 0}, {L/9 + deviation, L}};
	    lines[19] = (struct Line){L/9 - 2*deviation, 0.0,
                {L/9 - 2*deviation, 0}, {L/9 - 2*deviation, L}};
	    lines[20] = (struct Line){L/9 + 2*deviation, 0.0,
                {L/9 + 2*deviation, 0}, {L/9 + 2*deviation, L}};

	    // Noisy lines around the first horizontal line
	    lines[21] = (struct Line){0.0, 1.5708, {0, L/9 - deviation},
                {L, L/9 - deviation}};
	    lines[22] = (struct Line){0.0, 1.5708, {0, L/9}, {L, L/9}};
	    lines[23] = (struct Line){0.0, 1.5708, {0, L/9 + deviation},
                {L, L/9 + deviation}};
	    lines[23] = (struct Line){0.0, 1.5708, {0, L/9 - 2*deviation},
                {L, L/9 - 2*deviation}};
	    lines[25] = (struct Line){0.0, 1.5708, {0, L/9 + 2*deviation},
                {L, L/9 + 2*deviation}};



	    SDL_Texture* grayscale_texture =
                SDL_CreateTextureFromSurface(renderer, surface);
	    if (grayscale_texture == NULL)
		    errx(EXIT_FAILURE, "%s", SDL_GetError());

	    SDL_FreeSurface(surface);
	    event_loop_image_test_averagelines(renderer, grayscale_texture,
                    lines);
	    SDL_DestroyTexture(grayscale_texture);
    }

    /* PLEASE LEAVE THE EXAMPLE !
	       if (strcmp(argv[1], "FILTER NAME") == 0){
	       surface_to_"FILTER NAME"(surface);
	       SDL_Texture* "FILTER NAME"_texture =
               SDL_CreateTextureFromSurface(renderer, surface);
	       if ("FILTER NAME"_texture == NULL)
	       errx(EXIT_FAILURE, "%s", SDL_GetError());

	       SDL_FreeSurface(surface);
	       event_loop_image(renderer, "FILTER NAME"_texture);
	       SDL_DestroyTexture("FILTER NAME"_texture);

	       }
	       */

       else {
           errx(EXIT_FAILURE, "Unsupported filter: %s", argv[1]);
       }

       SDL_DestroyRenderer(renderer);
       SDL_DestroyWindow(window);
       SDL_Quit();
       return EXIT_SUCCESS;
}


//printf("i think Ali is the goat...")


