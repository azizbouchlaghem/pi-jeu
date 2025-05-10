#include "perso.h"
#include <SDL/SDL_image.h>
#include <stdio.h>

void initPerso(Personne *p) {
    p->imagePers = IMG_Load("perso.png");
    if (!p->imagePers) {
        printf("Erreur chargement sprite : %s\n", SDL_GetError());
        exit(1);
    }

    p->coeur_plein = IMG_Load("coeur_plein.png");
    p->coeur_vide = IMG_Load("coeur_vide.png");
    if (!p->coeur_plein || !p->coeur_vide) {
        printf("Erreur chargement cœurs : %s\n", SDL_GetError());
        exit(1);
    }

    p->position.x = 100;
    p->position.y = 800;
    p->frame.x = 0;
    p->frame.y = 0;
    p->frame.w = 64;
    p->frame.h = 64;
    p->curframe = 0;

    p->vitesse = 0.3;
    p->acceleration = 0.0;
    p->v_grav = 0.3;
    p->v_saut = -7.0;
    p->v_x = 0;
    p->v_y = 0;
    p->direction = 1;
    p->Vscore = 0;
    p->en_saut = 0;

    p->vie = 3;

    p->score.font = TTF_OpenFont("angelina.TTF", 24);
    if (!p->score.font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        exit(1);
    }

    p->score.color = (SDL_Color){255, 255, 255};
    p->score.postext.x = 30;
    p->score.postext.y = 30;
}

void movePerso(Personne *p, Uint32 dt) {
    float dt_seconds = dt / 1000.0;
    p->v_x = (p->direction == 1) ? p->vitesse : -p->vitesse;
    p->position.x += (int)(p->v_x * dt_seconds * 1000);
}

void animerPerso(Personne *p) {
    p->curframe++;
    if (p->curframe >= 4) p->curframe = 0;
    p->frame.x = p->curframe * 64;
}

void sautdroit(Personne *p) {
    if (p->en_saut) {
        p->v_y += p->v_grav;
        p->position.y += (int)p->v_y;

        if (p->position.y >= 800) {
            p->position.y = 800;
            p->v_y = 0;
            p->en_saut = 0;
        }
    }
}

void afficherPersoAvecPos(Personne p, SDL_Surface *screen, SDL_Rect pos) {
    SDL_BlitSurface(p.imagePers, &p.frame, screen, &pos);

    char scoreText[50];
    sprintf(scoreText, "Score: %d", p.Vscore);
    SDL_Surface* textSurface = TTF_RenderText_Blended(p.score.font, scoreText, p.score.color);
    if (textSurface) {
        SDL_BlitSurface(textSurface, NULL, screen, &p.score.postext);
        SDL_FreeSurface(textSurface);
    }
}

void afficherVie(Personne p, SDL_Surface *screen) {
    SDL_Rect pos;
    pos.y = 10;
    for (int i = 0; i < 3; i++) {
        pos.x = 10 + i * 40;
        if (i < p.vie)
            SDL_BlitSurface(p.coeur_plein, NULL, screen, &pos);
        else
            SDL_BlitSurface(p.coeur_vide, NULL, screen, &pos);
    }
}
