#ifndef MENU_H
#define MENU_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>

typedef enum {
    MENU_PRINCIPAL,
    SOUS_MENU_OPTIONS,
    SOUS_MENU_SAUVEGARDE,
    SOUS_MENU_JOUEUR,
    SOUS_MENU_AVATAR,
    SOUS_MENU_SCORES,
    SOUS_MENU_ENIGME,
    PLAYING_SOLO,
    PLAYING_MULTI,
    QUIT
} GameState;

typedef struct {
    SDL_Surface* normal;
    SDL_Surface* hover;
    SDL_Rect position;
    bool isHovered;
} Button;

void initMenu();
void handleEvent(SDL_Event event);
void updateScreen();
void cleanupMenu();
void libererBackgroundMenu();

extern GameState state;

#endif // MENU_H
