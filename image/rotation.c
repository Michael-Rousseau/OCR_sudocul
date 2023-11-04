#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "rotation.h"



SDL_Surface* RotateImage(SDL_Surface* image, double angledegree)
{
	float rad = angledegree * M_PI /180.0;
	double coco = cos(rad);
	double sisi = sin(rad);

	int w1 = image->w;
	int h1 = image->h;

	int w2 = fabs(coco * w1) + fabs(sisi *h1); //x' = xcos + ysin
	int h2 = fabs(sisi*w1) + fabs(coco*h1);

	SDL_Surface* rot = SDL_CreateRGBSurface(0,w2,h2,32,0,0,0,0);

	SDL_UnlockSurface(image);
	SDL_UnlockSurface(rot);


	//FAIRE MODULO 360 FMOD!!!!!!!!!!!!!!!! if 0 DO NOTHINGGGGGG

	for(int y = 0; y < h2; y++)
	{
		for(int x = 0; x < w2; x++)
		{
			int middlex = w1 / 2; //center coordinates of
                                              //the source image
			int middley = h1 / 2;


			//with the distance to the center for each pixe
                        //-> trigonometric calculations can be used
                        //(the rotation matrix)

			int distancex = x - w2 / 2;  // distance of
                                                     // current pixel
                // from the center of the destination image in the x direction.
			int distancey = y - w2 / 2;

			//the corresponding pixel in the source image
			int truex = middlex + distancex *coco - distancey *sisi;
			int truey = middley+ distancey *coco+ distancex *sisi;

			if (truex >= 0 && truey >=0 && truex <w1 && truey <h1)
				((Uint32*)rot->pixels)[y * w2 + x]= ((Uint32*)
                                    image->pixels)[truey * w1 + truex];
			else
				((Uint32*)rot->pixels)[y * w2 + x] = 0x000000;
                        //transparent sdl rgb
			}
	}

	SDL_UnlockSurface(image);
	SDL_UnlockSurface(rot);

	//memcpy(image->pixels, rot->pixels, w * h * sizeof(Uint32))

	return rot;

}




