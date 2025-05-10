#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

typedef struct struct_text {
    TTF_Font *font;
    SDL_Color color;
    SDL_Surface* text;
    SDL_Rect postext;
} Text;

typedef struct {
    SDL_Surface *imagePers;
    SDL_Rect position;
    SDL_Rect frame;
    int curframe;

    int direction;
    double acceleration;
    double vitesse;
    double v_x;
    double v_grav;
    double v_saut;
    double v_y;
    int en_saut;

    int Vscore;
    int vie;

    SDL_Surface *coeur_plein;
    SDL_Surface *coeur_vide;

    Text score;
} Personne;

// Fonctions
void initPerso(Personne *p);
void movePerso(Personne *p, Uint32 dt);
void animerPerso(Personne *p);
void sautdroit(Personne *p);
void afficherPersoAvecPos(Personne p, SDL_Surface *screen, SDL_Rect pos);
void afficherVie(Personne p, SDL_Surface *screen);

#endif
