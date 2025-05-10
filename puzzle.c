#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char *pieceFiles[NUM_PIECES] = {
    "correct.jpg",
    "incorrect1.jpg", 
    "incorrect2.jpg"
};

SDL_Surface* createPlaceholder(SDL_Surface *reference, int w, int h, Uint32 color) {
    SDL_Surface* surf = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
    if (surf && reference) {
        SDL_FillRect(surf, NULL, SDL_MapRGB(reference->format, 
                      (color >> 16) & 0xFF, 
                      (color >> 8) & 0xFF, 
                      color & 0xFF));
    }
    return surf;
}

void initialiserPuzzle(Puzzle *p, SDL_Surface *ecran) {
    // Initialize all surface pointers to NULL
    p->imageOriginale = NULL;
    for (int i = 0; i < NUM_PIECES; i++) {
        p->pieces[i] = NULL;
    }
    p->suivant = NULL;
    p->message = NULL;

    // Load images with error handling
    p->imageOriginale = IMG_Load("original.jpg");
    if (!p->imageOriginale) {
        fprintf(stderr, "Failed to load original.jpg: %s\n", IMG_GetError());
        p->imageOriginale = createPlaceholder(ecran, 300, 300, 0xFF00FF);
    }

    for (int i = 0; i < NUM_PIECES; i++) {
        p->pieces[i] = IMG_Load(pieceFiles[i]);
        if (!p->pieces[i]) {
            fprintf(stderr, "Failed to load %s: %s\n", pieceFiles[i], IMG_GetError());
            p->pieces[i] = createPlaceholder(ecran, 100, 100, 0xFF0000 + (i * 0x00AA00));
        }
    }

    p->suivant = IMG_Load("suivant.jpg");
    if (!p->suivant) {
        fprintf(stderr, "Failed to load suivant.jpg: %s\n", IMG_GetError());
        p->suivant = createPlaceholder(ecran, 100, 50, 0xFFFF00);
    }

    // Set positions
    p->positionImage.x = 300;
    p->positionImage.y = 150;

    // Target area
    p->positionCible.x = p->positionImage.x;
    p->positionCible.y = p->positionImage.y + (p->imageOriginale ? p->imageOriginale->h/2 : 150) - 
                         (p->pieces[0] ? p->pieces[0]->h/2 : 50);
    p->positionCible.w = p->pieces[0] ? p->pieces[0]->w : 100;
    p->positionCible.h = p->pieces[0] ? p->pieces[0]->h : 100;

    // Position draggable pieces
    for (int i = 0; i < NUM_PIECES; i++) {
        p->piecePositions[i].x = 50;
        p->piecePositions[i].y = 150 + i * 120;
        if (p->pieces[i]) {
            p->piecePositions[i].w = p->pieces[i]->w;
            p->piecePositions[i].h = p->pieces[i]->h;
        } else {
            p->piecePositions[i].w = 100;
            p->piecePositions[i].h = 100;
        }
    }

    // Button position
    p->positionSuivant.x = 700;
    p->positionSuivant.y = 400;
    if (p->suivant) {
        p->positionSuivant.w = p->suivant->w;
        p->positionSuivant.h = p->suivant->h;
    } else {
        p->positionSuivant.w = 100;
        p->positionSuivant.h = 50;
    }

    // Message position
    p->positionMessage.x = 350;
    p->positionMessage.y = 30;

    // Timer setup
    p->timerBg.x = 50;
    p->timerBg.y = 20;
    p->timerBg.w = 300;
    p->timerBg.h = 20;
    p->timerBar = p->timerBg;
    
    p->startTime = SDL_GetTicks();
    p->bravo = false;
    p->showMessage = false;
    p->timeUp = false;
    p->selectedPieceIndex = -1;
    p->correctPieceIndex = 0;
    p->autoSnapping = false;
}

void verifierPlacement(Puzzle *p) {
    if (p->timeUp) return;
    
    SDL_Rect *piece = &p->piecePositions[p->correctPieceIndex];
    
    // Check if piece is within target
    if (piece->x >= p->positionCible.x - 10 &&
        piece->x <= p->positionCible.x + 10 &&
        piece->y >= p->positionCible.y - 10 &&
        piece->y <= p->positionCible.y + 10) {
        
        p->bravo = true;
        TTF_Font *font = TTF_OpenFont("arial.ttf", 36);
        if (!font) {
            fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
            return;
        }
        SDL_Color green = {0, 255, 0};
        p->message = TTF_RenderText_Solid(font, "Piece correcte!", green);
        TTF_CloseFont(font);
        if (!p->message) {
            fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
        }
    }
    p->showMessage = true;
}

void afficherPuzzle(Puzzle *p, SDL_Surface *ecran) {
    // White background
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    
    // Draw timer
    Uint32 elapsed = SDL_GetTicks() - p->startTime;
    float progress = 1.0f;
    if (GAME_DURATION > 0) {
        progress = (GAME_DURATION - elapsed) / (float)GAME_DURATION;
        if (progress < 0) progress = 0;
    }
    
    p->timerBar.w = (int)(p->timerBg.w * progress);
    SDL_FillRect(ecran, &p->timerBg, SDL_MapRGB(ecran->format, 200, 200, 200));
    SDL_FillRect(ecran, &p->timerBar, SDL_MapRGB(ecran->format, 0, 255, 0));

    // Check if time is up
    if (!p->timeUp && elapsed >= GAME_DURATION) {
        p->timeUp = true;
        TTF_Font *font = TTF_OpenFont("arial.ttf", 36);
        if (font) {
            SDL_Color red = {255, 0, 0};
            p->message = TTF_RenderText_Solid(font, "Time's up!", red);
            TTF_CloseFont(font);
        }
    }

    // Draw original image
    if (p->imageOriginale) {
        SDL_BlitSurface(p->imageOriginale, NULL, ecran, &p->positionImage);
    }
    
    // Draw target highlight (pulsing effect)
    Uint32 ticks = SDL_GetTicks();
    Uint8 alpha = 100 + (Uint8)(100 * sin(ticks * 0.01f));
    SDL_Rect highlight = {
        p->positionCible.x - 5,
        p->positionCible.y - 5,
        p->positionCible.w + 10,
        p->positionCible.h + 10
    };
    SDL_FillRect(ecran, &highlight, SDL_MapRGBA(ecran->format, 0, 255, 0, alpha));
    
    // Draw all puzzle pieces
    for (int i = 0; i < NUM_PIECES; i++) {
        if (p->pieces[i]) {
            SDL_BlitSurface(p->pieces[i], NULL, ecran, &p->piecePositions[i]);
        }
    }
    
    // Draw button
    if (p->suivant) {
        SDL_BlitSurface(p->suivant, NULL, ecran, &p->positionSuivant);
    }

    // Draw message
    if (p->showMessage && p->message) {
        SDL_BlitSurface(p->message, NULL, ecran, &p->positionMessage);
    }
}

void updatePuzzle(SDL_Event event, Puzzle *p, SDL_Surface *ecran) {
    if (p->timeUp) return;
    
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                // Check suivant button
                if (mouseX >= p->positionSuivant.x && 
                    mouseX <= p->positionSuivant.x + p->positionSuivant.w &&
                    mouseY >= p->positionSuivant.y && 
                    mouseY <= p->positionSuivant.y + p->positionSuivant.h) {
                    verifierPlacement(p);
                }
                
                // Check pieces
                else if (!p->bravo) {
                    for (int i = 0; i < NUM_PIECES; i++) {
                        if (p->pieces[i] &&
                            mouseX >= p->piecePositions[i].x && 
                            mouseX <= p->piecePositions[i].x + p->piecePositions[i].w &&
                            mouseY >= p->piecePositions[i].y && 
                            mouseY <= p->piecePositions[i].y + p->piecePositions[i].h) {
                            p->selectedPieceIndex = i;
                            break;
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            p->selectedPieceIndex = -1;
            p->autoSnapping = false;
            break;

        case SDL_MOUSEMOTION:
            if (p->selectedPieceIndex >= 0 && p->selectedPieceIndex < NUM_PIECES && 
                p->pieces[p->selectedPieceIndex]) {
                SDL_Rect *piece = &p->piecePositions[p->selectedPieceIndex];
                int newX = event.motion.x - piece->w/2;
                int newY = event.motion.y - piece->h/2;
                
                // Boundary checking
                newX = (newX < 0) ? 0 : newX;
                newY = (newY < 0) ? 0 : newY;
                newX = (newX > SCREEN_WIDTH - piece->w) ? SCREEN_WIDTH - piece->w : newX;
                newY = (newY > SCREEN_HEIGHT - piece->h) ? SCREEN_HEIGHT - piece->h : newY;
                
                piece->x = newX;
                piece->y = newY;
                
                // Auto-snap when close to target (only for correct piece)
                if (p->selectedPieceIndex == p->correctPieceIndex) {
                    int distX = abs(piece->x + piece->w/2 - (p->positionCible.x + p->positionCible.w/2));
                    int distY = abs(piece->y + piece->h/2 - (p->positionCible.y + p->positionCible.h/2));
                    
                    if (distX < 50 && distY < 50) {  // 50px snap radius
                        p->autoSnapping = true;
                        // Smooth movement toward target
                        piece->x += (int)((p->positionCible.x - piece->x) * 0.2f);
                        piece->y += (int)((p->positionCible.y - piece->y) * 0.2f);
                    } else {
                        p->autoSnapping = false;
                    }
                }
            }
            break;
    }
}

void cleanUpPuzzle(Puzzle *p) {
    if (p->imageOriginale) SDL_FreeSurface(p->imageOriginale);
    for (int i = 0; i < NUM_PIECES; i++) {
        if (p->pieces[i]) SDL_FreeSurface(p->pieces[i]);
    }
    if (p->suivant) SDL_FreeSurface(p->suivant);
    if (p->message) SDL_FreeSurface(p->message);
}

int runPuzzleGame(SDL_Surface *ecran) {
    Puzzle puzzle;
    bool quitter = false;
    SDL_Event event;

    initialiserPuzzle(&puzzle, ecran);

    while (!quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = true;
                    break;
                default:
                    updatePuzzle(event, &puzzle, ecran);
                    break;
            }
        }

        afficherPuzzle(&puzzle, ecran);
        SDL_Flip(ecran);
        
        if ((puzzle.bravo || puzzle.timeUp) && puzzle.showMessage) {
            SDL_Delay(2000);
            quitter = true;
        }
        
        SDL_Delay(16);
    }

    cleanUpPuzzle(&puzzle);
    return 0;
}
