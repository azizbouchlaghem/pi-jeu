#include "map.h"
#include <SDL/SDL_video.h>

#define WORLD_WIDTH  4000
#define WORLD_HEIGHT 1024
#define MINIMAP_WIDTH  200
#define MINIMAP_HEIGHT 50

void initMinimap(Minimap *m) {
    m->miniSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, 32, 0, 0, 0, 0);
    m->pos.x = 0;
    m->pos.y = 0;
}

void updateMinimap(Minimap *m, SDL_Rect camera, SDL_Rect pos1, SDL_Rect pos2) {
    SDL_FillRect(m->miniSurface, NULL, SDL_MapRGB(m->miniSurface->format, 80, 80, 80)); // fond gris

    float scaleX = (float)MINIMAP_WIDTH / WORLD_WIDTH;
    float scaleY = (float)MINIMAP_HEIGHT / WORLD_HEIGHT;

    SDL_Rect miniP1 = {
        .x = pos1.x * scaleX,
        .y = pos1.y * scaleY,
        .w = 5,
        .h = 5
    };
    SDL_Rect miniP2 = {
        .x = pos2.x * scaleX,
        .y = pos2.y * scaleY,
        .w = 5,
        .h = 5
    };

    SDL_FillRect(m->miniSurface, &miniP1, SDL_MapRGB(m->miniSurface->format, 0, 255, 0)); // joueur 1 vert
    SDL_FillRect(m->miniSurface, &miniP2, SDL_MapRGB(m->miniSurface->format, 255, 0, 0)); // joueur 2 rouge
}

void afficherMinimap(Minimap *m, SDL_Surface *screen) {
    m->pos.x = screen->w - m->miniSurface->w - 20;
    m->pos.y = 20;

    // cadre noir
    SDL_Rect border = { m->pos.x - 2, m->pos.y - 2, m->miniSurface->w + 4, m->miniSurface->h + 4 };
    SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 0, 0, 0));

    SDL_BlitSurface(m->miniSurface, NULL, screen, &m->pos);
}
