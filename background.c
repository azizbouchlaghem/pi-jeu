#include "background.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

void initBackground(background *b) {
    b->imgback = IMG_Load("back-1.png");
    if (!b->imgback) {
        printf("IMG_Load: %s\n", IMG_GetError());
        exit(1);
    }
    // Initialize first view (left)
    b->camera1.x = 0;
    b->camera1.y = 0;
    b->camera1.w = SCREEN_W / 2;
    b->camera1.h = SCREEN_H;
    b->posEcran1.x = 0;
    b->posEcran1.y = 0;
    b->posEcran1.w = SCREEN_W / 2;
    b->posEcran1.h = SCREEN_H;

    // Initialize full screen position for single-player mode
    b->cameraFullScreen.x = 0;
    b->cameraFullScreen.y = 0;
    b->cameraFullScreen.w = SCREEN_W;
    b->cameraFullScreen.h = SCREEN_H;
    b->posEcranFull.x = 0;
    b->posEcranFull.y = 0;
    b->posEcranFull.w = SCREEN_W;
    b->posEcranFull.h = SCREEN_H;

    // Initialize second view (right)
    b->camera2.x = b->imgback->w / 2; // Important correction
    b->camera2.y = 0;
    b->camera2.w = SCREEN_W / 2;
    b->camera2.h = SCREEN_H;
    b->posEcran2.x = SCREEN_W / 2;
    b->posEcran2.y = 0;
    b->posEcran2.w = SCREEN_W / 2;
    b->posEcran2.h = SCREEN_H;

    b->pos_temp.x = 50;
    b->pos_temp.y = 50;
}

void afficheBackground(background b, SDL_Surface *ecran, int view) {
    if (view == 1) {
        SDL_BlitSurface(b.imgback, &b.camera1, ecran, &b.posEcran1);
    } else if (view == 2) {
        SDL_BlitSurface(b.imgback, &b.camera2, ecran, &b.posEcran2);
    } else if (view == 0) {
        SDL_BlitSurface(b.imgback, &b.cameraFullScreen, ecran, &b.posEcranFull);
    }
}

void scrolling(background *b, int view, int direction, int pas) {
    SDL_Rect *cam;
    int minX = 0, maxX = 0;

    if (view == 1) {
        cam = &b->camera1;
        minX = 0;
        maxX = b->imgback->w / 2 - cam->w;
    } else if (view == 2) {
        cam = &b->camera2;
        minX = b->imgback->w / 2;
        maxX = b->imgback->w - cam->w;
    } else {
        cam = &b->cameraFullScreen;
        minX = 0;
        maxX = b->imgback->w - cam->w;
    }

    switch (direction) {
        case 0: // right
            if (cam->x + pas <= maxX)
                cam->x += pas;
            break;
        case 1: // left
            if (cam->x - pas >= minX)
                cam->x -= pas;
            break;
        case 2: // up
            if (cam->y - pas >= 0)
                cam->y -= pas;
            break;
        case 3: // down
            if (cam->y + pas <= b->imgback->h - cam->h)
                cam->y += pas;
            break;
    }
}

void drawHintOverlay(SDL_Surface *screen) {
    SDL_Surface* hintOverlay = IMG_Load("hint.png");
    if (hintOverlay) {
        SDL_Rect pos;
        pos.x = (screen->w - hintOverlay->w) / 2;
        pos.y = 10;
        SDL_BlitSurface(hintOverlay, NULL, screen, &pos);
        SDL_FreeSurface(hintOverlay);
    }
}

void gestiontemp(background m, SDL_Surface *screen) {
    TTF_Init();
    SDL_Color couleur = {138, 138, 138};
    TTF_Font* police = TTF_OpenFont("angelina.TTF", 60);
    sprintf(m.temp, "tempsJ: %d s", m.temp_sec);
    m.temps = TTF_RenderText_Solid(police, m.temp, couleur);
    SDL_BlitSurface(m.temps, NULL, screen, &m.pos_temp);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_FreeSurface(m.temps);
}
