
// Includes
#include "menu.h"
#include "perso.h"
#include "background.h"
#include "ennemie.h"
#include "map.h"
#include "enigme.h"
#include "puzzle.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur Mix_OpenAudio: %s\n", Mix_GetError());
    }

    SDL_Surface* screen = SDL_SetVideoMode(1530, 1024, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Erreur SDL_SetVideoMode: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_WM_SetCaption("God's Tear", NULL);
    srand(time(NULL));

    // Initialisation
    initMenu();
    background bgJeu;
    initBackground(&bgJeu);

    Personne p1, p2;
    initPerso(&p1);
    initPerso(&p2);
    p2.position.x = bgJeu.camera2.x + 100;
    p2.position.y = 800;

    Ennemi ennemi;
    initEnnemi(&ennemi, SCREEN_W);

    Minimap minimap;
    initMinimap(&minimap);

    int mode = 0;
    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;
    bool running = true;
    static bool bgMenuFreed = false;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 dt = currentTime - lastTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (state != PLAYING_SOLO && state != PLAYING_MULTI)
                handleEvent(event);
        }

        if (!bgMenuFreed && (state == PLAYING_SOLO || state == PLAYING_MULTI)) {
            libererBackgroundMenu();
            bgMenuFreed = true;
        }

        if (state == PLAYING_SOLO || state == PLAYING_MULTI) {
            mode = (state == PLAYING_SOLO) ? 0 : 1;

            SDL_PumpEvents();
            const Uint8* keys = SDL_GetKeyState(NULL);
            int moving1 = 0, moving2 = 0;

            if (keys[SDLK_ESCAPE]) state = MENU_PRINCIPAL;

            if (keys[SDLK_SPACE] && !p1.en_saut) {
                p1.en_saut = 1;
                p1.v_y = p1.v_saut;
            }
            if (keys[SDLK_z] && !p2.en_saut) {
                p2.en_saut = 1;
                p2.v_y = p2.v_saut;
            }

            if (keys[SDLK_RIGHT]) {
                p1.direction = 1;
                movePerso(&p1, dt);
                moving1 = 1;
            } else if (keys[SDLK_LEFT]) {
                p1.direction = 0;
                movePerso(&p1, dt);
                moving1 = 1;
            }

            if (mode == 1) {
                if (keys[SDLK_d]) {
                    p2.direction = 1;
                    movePerso(&p2, dt);
                    moving2 = 1;
                } else if (keys[SDLK_q]) {
                    p2.direction = 0;
                    movePerso(&p2, dt);
                    moving2 = 1;
                }
            }

            if (moving1) animerPerso(&p1);
            if (mode == 1 && moving2) animerPerso(&p2);

            moveEnnemi(&ennemi, SCREEN_W);
            if (!ennemi.actif) initEnnemi(&ennemi, SCREEN_W);

            if (ennemi.actif && collisionAvecEnnemi(p1.position, ennemi.pos)) {
                if (p1.vie > 0) p1.vie--;
                ennemi.actif = 0;
                SDL_Delay(200);
            }

            if (mode == 1 && ennemi.actif && collisionAvecEnnemi(p2.position, ennemi.pos)) {
                if (p2.vie > 0) p2.vie--;
                ennemi.actif = 0;
                SDL_Delay(200);
            }

            if (p1.vie <= 0 || (mode == 1 && p2.vie <= 0)) {
                
// Load enigmes
Enigme enigme = chargerEnigmes("enigmes.txt");
TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
SDL_Color textColor = {255, 255, 255};

if (font && enigme.nombreQuestions > 0) {
    initialiserMenu(&enigme, font, textColor);
    enigme.inMenu = 1;

    bool choiceMade = false;
    bool choseEnigme = false;

    while (!choiceMade && running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            else if (event.type == SDL_MOUSEMOTION) 
                verifierHoverMenu(&enigme, event.motion.x, event.motion.y);
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int choice = verifierClicMenu(&enigme, event.button.x, event.button.y);
                if (choice == 1) { choiceMade = true; choseEnigme = true; }
                else if (choice == 2) { choiceMade = true; choseEnigme = false; }
            }
        }
        afficherMenu(screen, &enigme);
        SDL_Flip(screen);
    }

    if (choiceMade && choseEnigme) {
        genererEnigmeAleatoire(&enigme);
        initialiserSurfaces(&enigme, font, textColor);

        bool done = false;
        while (!done && running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int reponse = verifierClicReponse(&enigme, event.button.x, event.button.y);
                    if (reponse > 0) {
                        afficherEnigme(screen, &enigme, font, textColor);
                        SDL_Flip(screen);
                        SDL_Delay(1500);

                        if (enigme.indexActuel >= enigme.nombreQuestions) {
		    done = true; // All questions have been answered
		} else {
		    genererEnigmeAleatoire(&enigme);
		    initialiserSurfaces(&enigme, font, textColor);
		}
                    }
                }
            }
            afficherEnigme(screen, &enigme, font, textColor);
            animerEn(&enigme);
            afficherEn(&enigme, screen);
            SDL_Flip(screen);
        }

        if (enigme.score >= 5) {
            p1.vie = 3;
            if (mode == 1) p2.vie = 3;
            SDL_Surface* successMsg = TTF_RenderText_Blended(font, "Success! Health restored.", (SDL_Color){0, 255, 0});
            SDL_BlitSurface(successMsg, NULL, screen, NULL);
            SDL_Flip(screen);
            SDL_Delay(2000);
            SDL_FreeSurface(successMsg);
        } else {
            SDL_Surface* failMsg = TTF_RenderText_Blended(font, "Failed! Returning to menu.", (SDL_Color){255, 0, 0});
            SDL_BlitSurface(failMsg, NULL, screen, NULL);
            SDL_Flip(screen);
            SDL_Delay(2000);
            SDL_FreeSurface(failMsg);
            state = MENU_PRINCIPAL;
        }
    } 
    else if (choiceMade && !choseEnigme) {
        Puzzle puzzle;
        initialiserPuzzle(&puzzle, screen);

        while (!puzzle.bravo && !puzzle.timeUp && running) {
            afficherPuzzle(&puzzle, screen);
            SDL_Flip(screen);

            while (SDL_PollEvent(&event)) {
                updatePuzzle(event, &puzzle, screen);
                if (event.type == SDL_QUIT) running = false;
            }
        }

        if (puzzle.bravo) {
            p1.vie = 3;
            if (mode == 1) p2.vie = 3;
        } else {
            state = MENU_PRINCIPAL;
        }

        cleanUpPuzzle(&puzzle);
    }

    TTF_CloseFont(font);
    libererEnigme(&enigme);
    continue;
}

                continue;
            }

            // Rendering
            if (mode == 0) {
                afficheBackground(bgJeu, screen, 0);
                afficherEnnemi(ennemi, screen);
                SDL_Rect pos1 = {p1.position.x - bgJeu.cameraFullScreen.x, p1.position.y};
                afficherPersoAvecPos(p1, screen, pos1);
                afficherVie(p1, screen);
                updateMinimap(&minimap, bgJeu.cameraFullScreen, p1.position, p2.position);
                afficherMinimap(&minimap, screen);
            } else {
                afficheBackground(bgJeu, screen, 1);
                afficheBackground(bgJeu, screen, 2);
                afficherEnnemi(ennemi, screen);
                SDL_Rect pos1 = {p1.position.x - bgJeu.camera1.x + bgJeu.posEcran1.x, p1.position.y};
                SDL_Rect pos2 = {p2.position.x - bgJeu.camera2.x + bgJeu.posEcran2.x, p2.position.y};
                afficherPersoAvecPos(p1, screen, pos1);
                afficherPersoAvecPos(p2, screen, pos2);
                afficherVie(p1, screen);
                afficherVie(p2, screen);
                updateMinimap(&minimap, bgJeu.camera1, p1.position, p2.position);
                afficherMinimap(&minimap, screen);
            }

            SDL_Flip(screen);
            sautdroit(&p1);
            sautdroit(&p2);
        } else {
            updateScreen();
        }

        lastTime = currentTime;
        SDL_Delay(16);
    }

    cleanupMenu();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
