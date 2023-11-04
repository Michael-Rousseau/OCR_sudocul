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
			// rotation is typically performed relative to the center of the image
			int middlex = w1 / 2; //center coordinates of the source image
			int middley = h1 / 2;


			//with the distance to the center for each pixe -> trigonometric calculations can be used (the rotation matrix) 
			
			int distancex = x - w2 / 2;  // distance of current pixel from the center of the destination image in the x direction.
			int distancey = y - w2 / 2;

			//the corresponding pixel in the source image 
			int truex = middlex + distancex *coco - distancey *sisi;
			int truey = middley+ distancey *coco+ distancex *sisi;

			if (truex >= 0 && truey >=0 && truex <w1 && truey <h1)
				((Uint32*)rot->pixels)[y * w2 + x]= ((Uint32*)image->pixels)[truey * w1 + truex];
			else
				((Uint32*)rot->pixels)[y * w2 + x] = 0x000000; //transparent sdl rgb
			}
	}

	SDL_UnlockSurface(image);
	SDL_UnlockSurface(rot);

	//memcpy(image->pixels, rot->pixels, w * h * sizeof(Uint32))

	return rot;

}







/*
SDL_Surface* RotateImage(SDL_Surface* src, double angledegrees) 
{
	double rad = angledegrees * M_PI / 180.0;

	int width = src->w;
	int height = src->h;

	int maxw = fabs(width * cos(rad)) + fabs(height * sin(rad));
   	int maxh = fabs(height * cos(rad)) + fabs(width * sin(rad));

	SDL_Surface* rot = SDL_CreateRGBSurface(0, maxw, maxh, 32, 0, 0, 0, 0);

	int cx = width / 2;
	int cy = height / 2;

	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			// Translate point to the origin
			int xpo = x - cx;
			int ypo = y - cy;

			// Rotate with transformation matrix
			int xnew = (int)(xpo * cos(rad) - ypo * sin(rad));
			int ynew = (int)(xpo * sin(rad) + ypo * cos(rad));
			
			xnew += maxw/2;
			ynew += maxh/2;
			if (xnew >= 0 && xnew < width && ynew >= 0 && ynew < height) {
				// Set pixel color using nearest-neighbor interpolation
				Uint32* srcPixels = (Uint32*)src->pixels;
				Uint32* destPixels = (Uint32*)rot->pixels;
				destPixels[y * width + x] = srcPixels[ynew * width + xnew];
			}
		}
	}

	return rot;
}


 
   SDL_Surface* RotateImage(SDL_Surface* image, double angledegree)
{
	float angle = angledegree * (M_PI /180.0);
	double* multm[2] ={
		{cos(angle), -sin(angle)};
		{sin(angle), cos(angle)};
	};









SDL_Surface* RotateImage(SDL_Surface* image, double angledegree)

{
	int w = image->w;
	int h = image->h;

	float angle = angledegree * (M_PI / 180.0); // Convert angle to radians


	// Calculate new width and height after rotation
	//int neww = fabs(w * cos(angle)) + fabs((h-1) * sin(angle));
    	//int newh = fabs((w-1) * sin(angle)) + fabs((h-1) * cos(angle));
    	
	int neww = fabs(w * cos(angle)) + fabs(h * sin(angle));
   	int newh = fabs(h * cos(angle)) + fabs(w * sin(angle));

	SDL_Surface* rot = SDL_CreateRGBSurface(0, neww, newh, 32, 0, 0, 0, 0);

	SDL_LockSurface(image);
	SDL_LockSurface(rot);

	Uint32* srcPixels = (Uint32*)image->pixels;
	//Uint32* destPixels = (Uint32*)rot->pixels;


	double cx = w / 2.0;
   	double cy = h / 2.0;
	double nx = neww/2;
	double ny = newh/2;

	for (int y = 0; y < newh; y++)
	{
        	for (int x = 0; x < neww; x++) 
		{
           	 // Backward mapping
            		int srcx = (int)((x - nx) * cos(-angle) - (y - ny) * sin(-angle) + cx);
           		int srcy = (int)((x - nx) * sin(-angle) + (y - ny) * cos(-angle) + cy);

			if (srcx >=0 && srcy>=0 && srcx <= neww && srcy <= newh)
			{
				                Uint32* destPixels = (Uint32*)rot->pixels;

				        destPixels[y * neww + x] = srcPixels[srcy * w + srcx];
			}
		}
		
	}

	

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{

			// Translate the current pixel to have the center of the image at the origin
			float xtranslated = x - xcenter;
			float ytranslated = y - ycenter;

			// Apply the rotation formula
			int rotatedx = (int)(xtranslated * cos(angle) - ytranslated * sin(angle));
			int rotatedy = (int)(xtranslated * sin(angle) + ytranslated * cos(angle));

			// Translate back
			rotatedx += neww/2;
			rotatedy += newh/2;
			// if the new position is inside the image bounds
			if (rotatedy >= 0 && rotatedx < neww && rotatedx >= 0 && rotatedy < newh) 
				destPixels[rotatedy * neww + rotatedx] = srcPixels[y * w +x];
			
		}
	}
*/
	





