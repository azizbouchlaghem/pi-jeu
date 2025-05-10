#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>
#include "perso.h"

typedef struct {
    SDL_Surface *miniSurface;
    SDL_Rect pos;  // Position sur l'écran
} Minimap;

void initMinimap(Minimap *m);
void updateMinimap(Minimap *m, SDL_Rect camera, SDL_Rect pos1, SDL_Rect pos2);
void afficherMinimap(Minimap *m, SDL_Surface *screen);

#endif
