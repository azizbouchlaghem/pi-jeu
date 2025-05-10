#include "ennemie.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void initEnnemi(Ennemi *e, int x_start) {
    e->img = IMG_Load("ennemy_sheet.png");
    if (!e->img) {
        printf("Erreur chargement ennemi : %s\n", SDL_GetError());
        exit(1);
    }

    e->frame.w = 160; // largeur de sprite
    e->frame.h = 160; // hauteur de sprite
    e->frame.x = 0;
    e->frame.y = 0;
    e->frame_actuelle = 0;

    e->pos.x = x_start;
    e->pos.y = 700; // sol
    e->pos.w = e->frame.w;
    e->pos.h = e->frame.h;

    e->vitesse = 2;
    e->lastTime = SDL_GetTicks();
    e->actif = 1;
}

void afficherEnnemi(Ennemi e, SDL_Surface *screen) {
    SDL_BlitSurface(e.img, &e.frame, screen, &e.pos);
}

void animerEnnemi(Ennemi *e) {
    Uint32 now = SDL_GetTicks();
    if (now - e->lastTime > 100) {
        e->frame_actuelle = (e->frame_actuelle + 1) % 4; // nombre de frames
        e->frame.x = e->frame_actuelle * e->frame.w;
        e->lastTime = now;
    }
}

void deplacerEnnemi(Ennemi *e) {
    e->pos.x -= e->vitesse;
}

int ennemiHorsEcran(Ennemi e) {
    return (e.pos.x + e.pos.w < 0);
}

int collisionAvecEnnemi(SDL_Rect joueur, SDL_Rect ennemi) {
    if (joueur.x + joueur.w <= ennemi.x || joueur.x >= ennemi.x + ennemi.w)
        return 0;
    if (joueur.y + joueur.h <= ennemi.y || joueur.y >= ennemi.y + ennemi.h)
        return 0;
    return 1;
}
void moveEnnemi(Ennemi *e, int screen_w) {
    if (!e->actif) return;

    deplacerEnnemi(e);
    animerEnnemi(e);

    if (ennemiHorsEcran(*e)) {
        e->actif = 0;
    }
}

