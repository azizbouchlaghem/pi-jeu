#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "enigme.h"
#include "puzzle.h"

int main() {
    const int HAUTEUR = 1000;
    const int LARGEUR = 1800;
    int done = 1;
    SDL_Surface *ecran = NULL;
    SDL_Event event;
    TTF_Font *font = NULL;
    SDL_Color textColor = {255, 255, 255};
    Enigme enigme;
    int rep = -1;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur d'initialisation de TTF : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (ecran == NULL) {
        fprintf(stderr, "Erreur d'initialisation de la fenêtre : %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    SDL_WM_SetCaption("Enigme", NULL);
    
    font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "Erreur de chargement de la police : %s\n", TTF_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    enigme = chargerEnigmes("enigmes.txt");
    if (enigme.nombreQuestions == 0) {
        fprintf(stderr, "Aucune énigme n'a été chargée!\n");
        TTF_CloseFont(font);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    initialiserMenu(&enigme, font, textColor);
    enigme.inPuzzleGame = 0;
    
    while (done) {
        while (SDL_PollEvent(&event)) {
            if (enigme.inPuzzleGame) {
                switch (event.type) {
                    case SDL_QUIT:
                        done = 0;
                        break;
                    default:
                        updatePuzzle(event, &enigme.puzzleGame, ecran);
                        break;
                }
            } else {
                switch (event.type) {
                    case SDL_QUIT:
                        done = 0;
                        break;
                    case SDL_MOUSEMOTION:
                        if (enigme.inMenu) {
                            verifierHoverMenu(&enigme, event.motion.x, event.motion.y);
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (enigme.inMenu) {
                                int menuChoice = verifierClicMenu(&enigme, event.button.x, event.button.y);
                                if (menuChoice == 1) {
                                    genererEnigmeAleatoire(&enigme);
                                    initialiserSurfaces(&enigme, font, textColor);
                                } else if (menuChoice == 2) {
                                    enigme.inPuzzleGame = 1;
                                    initialiserPuzzle(&enigme.puzzleGame, ecran);
                                }
                            } else {
                                rep = verifierClicReponse(&enigme, event.button.x, event.button.y);
                                if (rep > 0) {
                                    printf("Vous avez cliqué sur la réponse %d\n", rep);
                                    gererReponse(&enigme, ecran, font, textColor, &done);
                                }
                            }
                        }
                        break;
                    case SDL_KEYDOWN:
                        if (!enigme.inMenu) {
                            if (event.key.keysym.sym == SDLK_a) {
                                rep = verifierClicReponse(&enigme, enigme.reponsePos[0].x + 10, enigme.reponsePos[0].y + 10);
                                if (rep > 0) {
                                    printf("Vous avez sélectionné la réponse A (1)\n");
                                    gererReponse(&enigme, ecran, font, textColor, &done);
                                }
                            }
                            else if (event.key.keysym.sym == SDLK_b) {
                                rep = verifierClicReponse(&enigme, enigme.reponsePos[1].x + 10, enigme.reponsePos[1].y + 10);
                                if (rep > 0) {
                                    printf("Vous avez sélectionné la réponse B (2)\n");
                                    gererReponse(&enigme, ecran, font, textColor, &done);
                                }
                            }
                            else if (event.key.keysym.sym == SDLK_c) {
                                rep = verifierClicReponse(&enigme, enigme.reponsePos[2].x + 10, enigme.reponsePos[2].y + 10);
                                if (rep > 0) {
                                    printf("Vous avez sélectionné la réponse C (3)\n");
                                    gererReponse(&enigme, ecran, font, textColor, &done);
                                }
                            }
                        }
                        break;
                }
            }
        }
        
        if (enigme.inPuzzleGame) {
            afficherPuzzle(&enigme.puzzleGame, ecran);
            if ((enigme.puzzleGame.bravo || enigme.puzzleGame.timeUp) && enigme.puzzleGame.showMessage) {
                SDL_Delay(2000);
                enigme.inPuzzleGame = 0;
                enigme.inMenu = 1;
            }
        } 
        else if (enigme.inMenu) {
            afficherMenu(ecran, &enigme);
        } 
        else {
            afficherEnigme(ecran, &enigme, font, textColor);
            animerEnnemi(&enigme);
            afficherEnnemi(&enigme, ecran);
        }
        
        SDL_Flip(ecran);
    }
    
    libererEnigme(&enigme);
    cleanUpPuzzle(&enigme.puzzleGame);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
