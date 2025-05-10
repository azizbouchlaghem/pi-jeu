#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 650
#define NUM_PIECES 3
#define GAME_DURATION 20000

typedef struct {
    SDL_Surface *imageOriginale;
    SDL_Surface *pieces[NUM_PIECES];
    SDL_Surface *suivant;
    SDL_Surface *message;

    SDL_Rect positionImage;
    SDL_Rect piecePositions[NUM_PIECES];
    SDL_Rect positionCible;
    SDL_Rect positionMessage;
    SDL_Rect positionSuivant;
    
    SDL_Rect timerBar;
    SDL_Rect timerBg;
    Uint32 startTime;

    bool bravo;
    bool showMessage;
    bool timeUp;
    int selectedPieceIndex;
    int correctPieceIndex;
    bool autoSnapping;
} Puzzle;

void initialiserPuzzle(Puzzle *p, SDL_Surface *ecran);
void afficherPuzzle(Puzzle *p, SDL_Surface *ecran);
void updatePuzzle(SDL_Event event, Puzzle *p, SDL_Surface *ecran);
void cleanUpPuzzle(Puzzle *p);
void verifierPlacement(Puzzle *p);
int runPuzzleGame(SDL_Surface *ecran);

#endif
