#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "puzzle.h"

#define MAX_QUESTIONS 10

typedef struct {
    char question[256];
    char reponse1[256];
    char reponse2[256];
    char reponse3[256];
    int numbr;
} Question;

typedef struct {
    Question questions[MAX_QUESTIONS];
    int nombreQuestions;
    int questionCourante;
    int ordreQuestions[MAX_QUESTIONS];
    int indexActuel;

    SDL_Surface *questionSurface;
    SDL_Surface *background;
    SDL_Surface *successMessage;
    SDL_Surface *failureMessage;
    SDL_Surface *progressBar;
    SDL_Surface *reponseSurfaces[3];
    SDL_Rect questionPos;
    SDL_Rect reponsePos[3];
    SDL_Rect messagePos;
    SDL_Rect progressPos;

    int score;
    int etat;
    
    SDL_Surface *image_EN;
    SDL_Rect pos_image_EN;
    SDL_Rect single_EN;
    int EN_num;
    Uint32 lastFrameTime;
    
    // Menu variables
    SDL_Surface *menuBackground;
    SDL_Surface *playButton;
    SDL_Surface *playButtonHover;
    SDL_Surface *settingsButton;
    SDL_Surface *settingsButtonHover;

    SDL_Rect playPos;
    SDL_Rect settingsPos;

    int inMenu;
    int isPlayHovered;
    int isSettingsHovered;
    
    // Puzzle game state
    Puzzle puzzleGame;
    int inPuzzleGame;
} Enigme;

Enigme chargerEnigmes(const char *nom_du_fichier);
void genererEnigmeAleatoire(Enigme *enigme);
void initialiserSurfaces(Enigme *enigme, TTF_Font *font, SDL_Color textColor);
void mettreAJourProgressBar(Enigme *enigme, TTF_Font *font);
void afficherEnigme(SDL_Surface *ecran, Enigme *enigme, TTF_Font *font, SDL_Color textColor);
int verifierClicReponse(Enigme *enigme, int x, int y);
void gererReponse(Enigme *enigme, SDL_Surface *ecran, TTF_Font *font, SDL_Color textColor, int *done);
void libererEnigme(Enigme *enigme);
void afficherQuestionNumero(SDL_Surface *ecran, Enigme *enigme, TTF_Font *font, SDL_Color textColor);
void animerEnnemi(Enigme *e);
void afficherEnnemi(Enigme* e, SDL_Surface *screen);
void initialiserMenu(Enigme *enigme, TTF_Font *font, SDL_Color textColor);
void afficherMenu(SDL_Surface *ecran, Enigme *enigme);
int verifierClicMenu(Enigme *enigme, int x, int y);
void verifierHoverMenu(Enigme *enigme, int x, int y);

#endif
