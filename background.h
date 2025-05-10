#ifndef BACKGROUND_H_INCLUDE
#define BACKGROUND_H_INCLUDE
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#define SCREEN_W 1360
#define SCREEN_H 1024

typedef struct background
{
    SDL_Surface *imgback;
    SDL_Rect camera1, posEcran1,pos_temp;  // First view
    SDL_Rect camera2, posEcran2;  // Second view
    SDL_Rect cameraFullScreen, posEcranFull; // Full screen view
    
    SDL_Surface * temps;
	  char temp[30];
	   int temp_sec;
} background;

void initBackground(background *b);
void afficheBackground(background b, SDL_Surface *ecran, int view); // view: 1 for left, 2 for right
void scrolling(background *b, int view, int direction, int pas);
void drawHintOverlay(SDL_Surface *screen);
void gestiontemp(background m,SDL_Surface * screen);

#endif
