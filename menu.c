#include "perso.h"
#include "menu.h"
#include <string.h>
#include <stdio.h>
SDL_Surface* loadImage(const char* path);
void initButtonsMenuPrincipal();
void initButtonsMenuOptions();
void initButtonsMenuSauvegarde();
void initButtonsMenuJoueur();
void initButtonsMenuEnigme();
static int screenWidth = 800;
static int screenHeight = 600;
static bool isFullscreen = false;
// Variables globales
GameState state = MENU_PRINCIPAL;
static GameState prochainEtat = MENU_PRINCIPAL;

// Surfaces et sons
static SDL_Surface* screen = NULL;
static SDL_Surface* background = NULL;
static Mix_Music* music = NULL;
static Mix_Chunk* hoverSound = NULL;
static Mix_Music* victoryMusic = NULL;
static Mix_Music* suspenseMusic = NULL;

// Structures de boutons
#define NUM_BUTTONS_MENU 5
static Button buttonsMenuPrincipal[NUM_BUTTONS_MENU];

#define NUM_BUTTONS_OPTIONS 5
static Button buttonsMenuOptions[NUM_BUTTONS_OPTIONS];

#define NUM_BUTTONS_SAUVEGARDE 2
#define NUM_BUTTONS_CHOIX 2
static Button buttonsMenuSauvegarde[NUM_BUTTONS_SAUVEGARDE];
static Button buttonsMenuChoix[NUM_BUTTONS_CHOIX];
static bool choixSauvegarde = false;

#define NUM_BUTTONS_JOUEUR 2
#define NUM_BUTTONS_AVATAR 4
static Button buttonsMenuJoueur[NUM_BUTTONS_JOUEUR];
static Button buttonsChoixAvatar[NUM_BUTTONS_AVATAR];
static bool choisirAvatar = false;

#define NUM_BUTTONS_ENIGME 2
#define NUM_BUTTONS_QUIZ 3
static Button buttonsMenuEnigme[NUM_BUTTONS_ENIGME];
static Button buttonsQuiz[NUM_BUTTONS_QUIZ];
static bool inQuiz = false;

// Fonctions utilitaires
SDL_Surface* loadImage(const char* path) {
    SDL_Surface* img = IMG_Load(path);
    if (!img) {
        printf("Erreur chargement image : %s\n", path);
    }
    return img;
}

// Fonction pour initialiser un bouton
void initButton(Button* button, const char* normalPath, const char* hoverPath, int x, int y) {
    button->normal = loadImage(normalPath);
    button->hover = loadImage(hoverPath);
    button->position.x = x;
    button->position.y = y;
    button->isHovered = false;
}

// Initialisation de tous les boutons
void initButtonsMenuPrincipal() {
    const char* normals[NUM_BUTTONS_MENU] = {"jouer.png", "options.png", "scores.png", "histoire.png", "quitter.png"};
    const char* hovers[NUM_BUTTONS_MENU] = {"jouer_hover.png", "options_hover.png", "scores_hover.png", "histoire_hover.png", "quitter_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_MENU; i++) {
        initButton(&buttonsMenuPrincipal[i], normals[i], hovers[i], 300, 150 + i * 80);
    }
}

void initButtonsMenuOptions() {
    const char* normals[NUM_BUTTONS_OPTIONS] = {"plus.png", "moins.png", "fullscreen.png", "normal.png", "retour.png"};
    const char* hovers[NUM_BUTTONS_OPTIONS] = {"plus_hover.png", "moins_hover.png", "fullscreen_hover.png", "normal_hover.png", "retour_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_OPTIONS; i++) {
        initButton(&buttonsMenuOptions[i], normals[i], hovers[i], 300, 100 + i * 80);
    }
}

void initButtonsMenuSauvegarde() {
    const char* normals[NUM_BUTTONS_SAUVEGARDE] = {"oui.png", "non.png"};
    const char* hovers[NUM_BUTTONS_SAUVEGARDE] = {"oui_hover.png", "non_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_SAUVEGARDE; i++) {
        initButton(&buttonsMenuSauvegarde[i], normals[i], hovers[i], 300, 200 + i * 100);
    }
    const char* choixNormals[NUM_BUTTONS_CHOIX] = {"charger.png", "nouvelle_partie.png"};
    const char* choixHovers[NUM_BUTTONS_CHOIX] = {"charger_hover.png", "nouvelle_partie_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_CHOIX; i++) {
        initButton(&buttonsMenuChoix[i], choixNormals[i], choixHovers[i], 300, 200 + i * 100);
    }
}

void initButtonsMenuJoueur() {
    const char* normals[NUM_BUTTONS_JOUEUR] = {"mono.png", "multi.png"};
    const char* hovers[NUM_BUTTONS_JOUEUR] = {"mono_hover.png", "multi_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_JOUEUR; i++) {
        initButton(&buttonsMenuJoueur[i], normals[i], hovers[i], 300, 200 + i * 100);
    }
    const char* avatarNormals[NUM_BUTTONS_AVATAR] = {"avatar1.png", "avatar2.png", "input1.png", "input2.png"};
    const char* avatarHovers[NUM_BUTTONS_AVATAR] = {"avatar1_hover.png", "avatar2_hover.png", "input1_hover.png", "input2_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
        initButton(&buttonsChoixAvatar[i], avatarNormals[i], avatarHovers[i], 150 + (i % 2) * 300, 400 + (i / 2) * 100);
    }
}

void initButtonsMenuEnigme() {
    const char* normals[NUM_BUTTONS_ENIGME] = {"quiz.png", "puzzle.png"};
    const char* hovers[NUM_BUTTONS_ENIGME] = {"quiz_hover.png", "puzzle_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_ENIGME; i++) {
        initButton(&buttonsMenuEnigme[i], normals[i], hovers[i], 300, 200 + i * 100);
    }
    const char* quizNormals[NUM_BUTTONS_QUIZ] = {"reponse1.png", "reponse2.png", "reponse3.png"};
    const char* quizHovers[NUM_BUTTONS_QUIZ] = {"reponse1_hover.png", "reponse2_hover.png", "reponse3_hover.png"};
    for (int i = 0; i < NUM_BUTTONS_QUIZ; i++) {
        initButton(&buttonsQuiz[i], quizNormals[i], quizHovers[i], 200, 300 + i * 80);
    }
}
void reloadUI() {
    // Recharger le background
    if (background) SDL_FreeSurface(background);
    background = loadImage("background1.png");

    // Réinitialiser tous les boutons
    initButtonsMenuPrincipal();
    initButtonsMenuOptions();
    initButtonsMenuSauvegarde();
    initButtonsMenuJoueur();
    initButtonsMenuEnigme();
}

void toggleFullscreen(bool enable) {
    Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

    if (enable) {
        flags |= SDL_NOFRAME;
        screenWidth = 1024;
        screenHeight = 768;
        isFullscreen = true;
    } else {
        // Important : réinitialiser le caption pour forcer le retour des bordures
        SDL_WM_SetCaption("Gods Tear", NULL);
        screenWidth = 800;
        screenHeight = 600;
        isFullscreen = false;
    }

    screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, flags);
    if (!screen) {
        fprintf(stderr, "Erreur en changeant le mode vidéo : %s\n", SDL_GetError());
    }

    reloadUI();
}



// Initialisation globale du menu
void initMenu() {
    screen = SDL_GetVideoSurface();
    background = loadImage("background1.png");

    music = Mix_LoadMUS("music1.mp3");
    if (music) Mix_PlayMusic(music, -1);
    Mix_Volume(-1, 128);
    hoverSound = Mix_LoadWAV("hover.wav");

    initButtonsMenuPrincipal();
    initButtonsMenuOptions();
    initButtonsMenuSauvegarde();
    initButtonsMenuJoueur();
    initButtonsMenuEnigme();
}
void handleButtonHover(Button* button, int mouseX, int mouseY) {
    bool previouslyHovered = button->isHovered;

    if (mouseX >= button->position.x && mouseX <= (button->position.x + button->normal->w) &&
        mouseY >= button->position.y && mouseY <= (button->position.y + button->normal->h)) {
        button->isHovered = true;
        if (!previouslyHovered && hoverSound) {
            Mix_PlayChannel(-1, hoverSound, 0);
        }
    } else {
        button->isHovered = false;
    }
}

void handleEvent(SDL_Event event) {
    if (state == MENU_PRINCIPAL) {
        if (event.type == SDL_MOUSEMOTION) {
            int x = event.motion.x;
            int y = event.motion.y;
            for (int i = 0; i < NUM_BUTTONS_MENU; i++) {
                handleButtonHover(&buttonsMenuPrincipal[i], x, y);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            for (int i = 0; i < NUM_BUTTONS_MENU; i++) {
                if (buttonsMenuPrincipal[i].isHovered) {
                    switch (i) {
                        case 0: state = SOUS_MENU_SAUVEGARDE; break; // Jouer
                        case 1: state = SOUS_MENU_OPTIONS; break;    // Options
                        case 2: state = SOUS_MENU_SCORES; break;     // Scores
                        case 3: /* Histoire - pas encore */ break;
                        case 4: state = QUIT; break;                 // Quitter
                    }
                }
            }
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_j: state = SOUS_MENU_SAUVEGARDE; break;
                case SDLK_o: state = SOUS_MENU_OPTIONS; break;
                case SDLK_m: state = SOUS_MENU_SCORES; break;
                case SDLK_ESCAPE: state = QUIT; break;
            }
        }
    }

    else if (state == SOUS_MENU_OPTIONS) {
        if (event.type == SDL_MOUSEMOTION) {
            int x = event.motion.x;
            int y = event.motion.y;
            for (int i = 0; i < NUM_BUTTONS_OPTIONS; i++) {
                handleButtonHover(&buttonsMenuOptions[i], x, y);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            for (int i = 0; i < NUM_BUTTONS_OPTIONS; i++) {
                if (buttonsMenuOptions[i].isHovered) {
                    switch (i) {
                        case 0: Mix_VolumeMusic(Mix_VolumeMusic(-1) + 10); break; // Volume +
                        case 1: Mix_VolumeMusic(Mix_VolumeMusic(-1) - 10); break; // Volume -
                        case 2: toggleFullscreen(true); break;          // Plein écran
                        case 3: toggleFullscreen(true); break; // Normal
                        case 4: state = MENU_PRINCIPAL; break;                   // Retour
                    }
                }
            }
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            state = MENU_PRINCIPAL;
        }
    }

    else if (state == SOUS_MENU_SAUVEGARDE) {
        if (!choixSauvegarde) {
            if (event.type == SDL_MOUSEMOTION) {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUM_BUTTONS_SAUVEGARDE; i++) {
                    handleButtonHover(&buttonsMenuSauvegarde[i], x, y);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < NUM_BUTTONS_SAUVEGARDE; i++) {
                    if (buttonsMenuSauvegarde[i].isHovered) {
                        if (i == 0) choixSauvegarde = true; // Oui
                        else state = MENU_PRINCIPAL;        // Non
                    }
                }
            }
        }
        else {
            if (event.type == SDL_MOUSEMOTION) {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUM_BUTTONS_CHOIX; i++) {
                    handleButtonHover(&buttonsMenuChoix[i], x, y);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < NUM_BUTTONS_CHOIX; i++) {
                    if (buttonsMenuChoix[i].isHovered) {
                        if (i == 0) state = MENU_PRINCIPAL;   // Charger jeu (futur)
                        else if (i == 1) state = SOUS_MENU_JOUEUR; // Nouvelle partie
                    }
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n) {
                state = SOUS_MENU_JOUEUR;
            }
        }
    }

    


else if (state == SOUS_MENU_JOUEUR) {
    if (event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;
        for (int i = 0; i < NUM_BUTTONS_JOUEUR; i++) {
            handleButtonHover(&buttonsMenuJoueur[i], x, y);
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        for (int i = 0; i < NUM_BUTTONS_JOUEUR; i++) {
            if (buttonsMenuJoueur[i].isHovered) {
                if (i == 0) prochainEtat = PLAYING_SOLO;
                else if (i == 1) prochainEtat = PLAYING_MULTI;
                state = SOUS_MENU_AVATAR; // ← interface avatar commune
            }
        }
    }
}

else if (state == SOUS_MENU_AVATAR) {
    if (event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;
        for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
            handleButtonHover(&buttonsChoixAvatar[i], x, y);
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
            if (buttonsChoixAvatar[i].isHovered) {
                // on peut ajouter des flags si tu veux stocker le choix
            }
        }
    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
        state = prochainEtat; // ← lance le vrai mode de jeu
    }
}

    else if (state == SOUS_MENU_SCORES) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_e) {
                state = SOUS_MENU_ENIGME;
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE) {
                state = MENU_PRINCIPAL;
            }
        }
    }

    else if (state == SOUS_MENU_ENIGME) {
        if (!inQuiz) {
            if (event.type == SDL_MOUSEMOTION) {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUM_BUTTONS_ENIGME; i++) {
                    handleButtonHover(&buttonsMenuEnigme[i], x, y);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < NUM_BUTTONS_ENIGME; i++) {
                    if (buttonsMenuEnigme[i].isHovered) {
                        if (i == 0) inQuiz = true;  // Quiz
                        else if (i == 1) state = MENU_PRINCIPAL; // Puzzle : pas encore implémenté
                    }
                }
            }
        }
        else {
            if (event.type == SDL_MOUSEMOTION) {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUM_BUTTONS_QUIZ; i++) {
                    handleButtonHover(&buttonsQuiz[i], x, y);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < NUM_BUTTONS_QUIZ; i++) {
                    if (buttonsQuiz[i].isHovered) {
                        state = MENU_PRINCIPAL; // Fin du quiz
                    }
                }
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            state = MENU_PRINCIPAL;
        }
    }
}
void updateScreen() {
    // Choisir l'arrière-plan selon le sous-menu
    if (state == MENU_PRINCIPAL) {
        background = loadImage("background1.png");
    }
    else if (state == SOUS_MENU_OPTIONS || state == SOUS_MENU_SAUVEGARDE || state == SOUS_MENU_JOUEUR) {
        background = loadImage("background2.png");
    }
    else if (state == SOUS_MENU_SCORES) {
        background = loadImage("background3.png");
    }
    else if (state == SOUS_MENU_ENIGME) {
        background = loadImage("background4.png");
    }

    else if (state == SOUS_MENU_AVATAR)  {
        background = loadImage("background2.png");
    }
    SDL_BlitSurface(background, NULL, screen, NULL);

    // Affichage des boutons
    if (state == MENU_PRINCIPAL) {
        for (int i = 0; i < NUM_BUTTONS_MENU; i++) {
            SDL_BlitSurface(buttonsMenuPrincipal[i].isHovered ? buttonsMenuPrincipal[i].hover : buttonsMenuPrincipal[i].normal,
                            NULL, screen, &buttonsMenuPrincipal[i].position);
        }
    }
    else if (state == SOUS_MENU_OPTIONS) {
        for (int i = 0; i < NUM_BUTTONS_OPTIONS; i++) {
            SDL_BlitSurface(buttonsMenuOptions[i].isHovered ? buttonsMenuOptions[i].hover : buttonsMenuOptions[i].normal,
                            NULL, screen, &buttonsMenuOptions[i].position);
        }
    }
    else if (state == SOUS_MENU_SAUVEGARDE) {
        if (!choixSauvegarde) {
            for (int i = 0; i < NUM_BUTTONS_SAUVEGARDE; i++) {
                SDL_BlitSurface(buttonsMenuSauvegarde[i].isHovered ? buttonsMenuSauvegarde[i].hover : buttonsMenuSauvegarde[i].normal,
                                NULL, screen, &buttonsMenuSauvegarde[i].position);
            }
        } else {
            for (int i = 0; i < NUM_BUTTONS_CHOIX; i++) {
                SDL_BlitSurface(buttonsMenuChoix[i].isHovered ? buttonsMenuChoix[i].hover : buttonsMenuChoix[i].normal,
                                NULL, screen, &buttonsMenuChoix[i].position);
            }
        }
    }
    else if (state == SOUS_MENU_JOUEUR) {
        if (!choisirAvatar) {
            for (int i = 0; i < NUM_BUTTONS_JOUEUR; i++) {
                SDL_BlitSurface(buttonsMenuJoueur[i].isHovered ? buttonsMenuJoueur[i].hover : buttonsMenuJoueur[i].normal,
                                NULL, screen, &buttonsMenuJoueur[i].position);
            }
        } else {
            for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
                SDL_BlitSurface(buttonsChoixAvatar[i].isHovered ? buttonsChoixAvatar[i].hover : buttonsChoixAvatar[i].normal,
                                NULL, screen, &buttonsChoixAvatar[i].position);
            }
        }
    }
    else if (state == SOUS_MENU_AVATAR) {
           for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
                SDL_BlitSurface(buttonsChoixAvatar[i].isHovered ? buttonsChoixAvatar[i].hover : buttonsChoixAvatar[i].normal,
                                NULL, screen, &buttonsChoixAvatar[i].position);
            }
         }
    else if (state == SOUS_MENU_SCORES) {
        TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
        if (font) {
            SDL_Color color = {255, 255, 255};
            SDL_Surface* message = TTF_RenderText_Blended(font, "Top 3 Scores :", color);
            SDL_Rect pos = {300, 100};
            SDL_BlitSurface(message, NULL, screen, &pos);
            SDL_FreeSurface(message);
            TTF_CloseFont(font);
        }
    }
    else if (state == SOUS_MENU_ENIGME) {
        if (!inQuiz) {
            for (int i = 0; i < NUM_BUTTONS_ENIGME; i++) {
                SDL_BlitSurface(buttonsMenuEnigme[i].isHovered ? buttonsMenuEnigme[i].hover : buttonsMenuEnigme[i].normal,
                                NULL, screen, &buttonsMenuEnigme[i].position);
            }
        } else {
            TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
            if (font) {
                SDL_Color color = {255, 255, 255};
                SDL_Surface* question = TTF_RenderText_Blended(font, "Quelle est la bonne réponse ?", color);
                SDL_Rect qPos = {200, 100};
                SDL_BlitSurface(question, NULL, screen, &qPos);
                SDL_FreeSurface(question);
                TTF_CloseFont(font);
            }

            for (int i = 0; i < NUM_BUTTONS_QUIZ; i++) {
                SDL_BlitSurface(buttonsQuiz[i].isHovered ? buttonsQuiz[i].hover : buttonsQuiz[i].normal,
                                NULL, screen, &buttonsQuiz[i].position);
            }
        }
    }

    SDL_Flip(screen);
}
void cleanupMenu() {
    if (background) SDL_FreeSurface(background);
    if (music) Mix_FreeMusic(music);
    if (hoverSound) Mix_FreeChunk(hoverSound);
    if (victoryMusic) Mix_FreeMusic(victoryMusic);
    if (suspenseMusic) Mix_FreeMusic(suspenseMusic);

    for (int i = 0; i < NUM_BUTTONS_MENU; i++) {
        if (buttonsMenuPrincipal[i].normal) SDL_FreeSurface(buttonsMenuPrincipal[i].normal);
        if (buttonsMenuPrincipal[i].hover) SDL_FreeSurface(buttonsMenuPrincipal[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_OPTIONS; i++) {
        if (buttonsMenuOptions[i].normal) SDL_FreeSurface(buttonsMenuOptions[i].normal);
        if (buttonsMenuOptions[i].hover) SDL_FreeSurface(buttonsMenuOptions[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_SAUVEGARDE; i++) {
        if (buttonsMenuSauvegarde[i].normal) SDL_FreeSurface(buttonsMenuSauvegarde[i].normal);
        if (buttonsMenuSauvegarde[i].hover) SDL_FreeSurface(buttonsMenuSauvegarde[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_CHOIX; i++) {
        if (buttonsMenuChoix[i].normal) SDL_FreeSurface(buttonsMenuChoix[i].normal);
        if (buttonsMenuChoix[i].hover) SDL_FreeSurface(buttonsMenuChoix[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_JOUEUR; i++) {
        if (buttonsMenuJoueur[i].normal) SDL_FreeSurface(buttonsMenuJoueur[i].normal);
        if (buttonsMenuJoueur[i].hover) SDL_FreeSurface(buttonsMenuJoueur[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_AVATAR; i++) {
        if (buttonsChoixAvatar[i].normal) SDL_FreeSurface(buttonsChoixAvatar[i].normal);
        if (buttonsChoixAvatar[i].hover) SDL_FreeSurface(buttonsChoixAvatar[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_ENIGME; i++) {
        if (buttonsMenuEnigme[i].normal) SDL_FreeSurface(buttonsMenuEnigme[i].normal);
        if (buttonsMenuEnigme[i].hover) SDL_FreeSurface(buttonsMenuEnigme[i].hover);
    }
    for (int i = 0; i < NUM_BUTTONS_QUIZ; i++) {
        if (buttonsQuiz[i].normal) SDL_FreeSurface(buttonsQuiz[i].normal);
        if (buttonsQuiz[i].hover) SDL_FreeSurface(buttonsQuiz[i].hover);
    }
}

void libererBackgroundMenu(){
	if (background){
	SDL_FreeSurface(background);
	background = NULL;
	}
}
