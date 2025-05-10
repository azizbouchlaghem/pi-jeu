#ifndef ENNEMIE_H
#define ENNEMIE_H

#include <SDL/SDL.h>

typedef struct {
    SDL_Surface *img;
    SDL_Rect pos;       // Position sur l'écran
    SDL_Rect frame;     // Partie de la spritesheet affichée
    int frame_actuelle;
    int vitesse;
    Uint32 lastTime;
    int actif;
} Ennemi;

// Initialisation
void initEnnemi(Ennemi *e, int x_start);

// Affichage
void afficherEnnemi(Ennemi e, SDL_Surface *screen);

// Animation
void animerEnnemi(Ennemi *e);

// Déplacement automatique
void deplacerEnnemi(Ennemi *e);

// Teste si l’ennemi est sorti de l’écran
int ennemiHorsEcran(Ennemi e);

// Collision rectangle-rectangle
int collisionAvecEnnemi(SDL_Rect joueur, SDL_Rect ennemi);

void moveEnnemi(Ennemi *e, int screen_w);

#endif
