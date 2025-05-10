#include "enigme.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void initialiserMenu(Enigme *enigme, TTF_Font *font, SDL_Color textColor) {
    // Charger l'arrière-plan du menu
    enigme->menuBackground = IMG_Load("background3.png");
    if (!enigme->menuBackground) {
        fprintf(stderr, "Impossible de charger l'image de fond du menu: %s\n", IMG_GetError());
        SDL_Surface *screen = SDL_GetVideoSurface();
        enigme->menuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
        SDL_FillRect(enigme->menuBackground, NULL, SDL_MapRGB(screen->format, 0, 0, 50));
    }

    // Charger les images des boutons
    enigme->playButton = IMG_Load("quiz.png");
    enigme->playButtonHover = IMG_Load("quiz_hover.png");
    enigme->settingsButton = IMG_Load("puzzle.png");
    enigme->settingsButtonHover = IMG_Load("puzzle_hover.png");
    
    if (!enigme->playButton || !enigme->playButtonHover || 
        !enigme->settingsButton || !enigme->settingsButtonHover) {
        fprintf(stderr, "Erreur de chargement des images de boutons: %s\n", IMG_GetError());
    }

    // Positionner les boutons sur la même ligne avec 100px d'espace
    int centerX = 600;  // Centre de l'écran (largeur 1200)
    int buttonY = 400;  // Position Y commune
    
    // Bouton Play
    enigme->playPos.x = centerX - 150 - (enigme->playButton ? enigme->playButton->w : 100);
    enigme->playPos.y = buttonY;
    enigme->playPos.w = enigme->playButton ? enigme->playButton->w : 100;
    enigme->playPos.h = enigme->playButton ? enigme->playButton->h : 50;

    // Bouton Settings
    enigme->settingsPos.x = centerX + 50;
    enigme->settingsPos.y = buttonY;
    enigme->settingsPos.w = enigme->settingsButton ? enigme->settingsButton->w : 100;
    enigme->settingsPos.h = enigme->settingsButton ? enigme->settingsButton->h : 50;

    enigme->inMenu = 1;
    enigme->isPlayHovered = 0;
    enigme->isSettingsHovered = 0;
}
void afficherMenu(SDL_Surface *ecran, Enigme *enigme) {
    // Afficher l'arrière-plan du menu
    SDL_BlitSurface(enigme->menuBackground, NULL, ecran, NULL);

    // Afficher les boutons avec état hover si nécessaire
    if (enigme->isPlayHovered && enigme->playButtonHover) {
        SDL_BlitSurface(enigme->playButtonHover, NULL, ecran, &enigme->playPos);
    } else if (enigme->playButton) {
        SDL_BlitSurface(enigme->playButton, NULL, ecran, &enigme->playPos);
    }

    if (enigme->isSettingsHovered && enigme->settingsButtonHover) {
        SDL_BlitSurface(enigme->settingsButtonHover, NULL, ecran, &enigme->settingsPos);
    } else if (enigme->settingsButton) {
        SDL_BlitSurface(enigme->settingsButton, NULL, ecran, &enigme->settingsPos);
    }
}





int verifierClicMenu(Enigme *enigme, int x, int y) {
    // Vérifier le clic sur le bouton Play
    if (x >= enigme->playPos.x && x < enigme->playPos.x + enigme->playPos.w &&
        y >= enigme->playPos.y && y < enigme->playPos.y + enigme->playPos.h) {
        enigme->inMenu = 0; // Quitter le menu
        return 1; // Play
    }

    // Vérifier le clic sur le bouton Settings
    if (x >= enigme->settingsPos.x && x < enigme->settingsPos.x + enigme->settingsPos.w &&
        y >= enigme->settingsPos.y && y < enigme->settingsPos.y + enigme->settingsPos.h) {
        printf("Settings clicked\n");
        return 2; // Settings
    }

    return 0; // Aucun bouton cliqué
}
void verifierHoverMenu(Enigme *enigme, int x, int y) {
    // Vérifier le hover sur le bouton Play
    enigme->isPlayHovered = (x >= enigme->playPos.x && 
                            x < enigme->playPos.x + enigme->playPos.w &&
                            y >= enigme->playPos.y && 
                            y < enigme->playPos.y + enigme->playPos.h);

    // Vérifier le hover sur le bouton Settings
    enigme->isSettingsHovered = (x >= enigme->settingsPos.x && 
                               x < enigme->settingsPos.x + enigme->settingsPos.w &&
                               y >= enigme->settingsPos.y && 
                               y < enigme->settingsPos.y + enigme->settingsPos.h);
}








Enigme chargerEnigmes(const char *nom_du_fichier) {
    Enigme e;
    FILE *f = NULL;
    int i = 0;

    // Initialiser la structure
    e.nombreQuestions = 0;
    e.questionCourante = -1;
    e.questionSurface = NULL;
    e.background = NULL;
    e.successMessage = NULL;
    e.failureMessage = NULL;
    e.progressBar = NULL;
    e.score = 0;
    e.etat = 0;

    for (int j = 0; j < 3; j++) {
        e.reponseSurfaces[j] = NULL;
    }

    // Ouvrir le fichier
    f = fopen(nom_du_fichier, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", nom_du_fichier);
        return e;
    }

    // Lire les questions du fichier (maximum MAX_QUESTIONS)
    while (i < MAX_QUESTIONS &&
           fscanf(f, "%255[^;];%255[^;];%255[^;];%255[^;];%d\n",
                  e.questions[i].question, e.questions[i].reponse1,
                  e.questions[i].reponse2, e.questions[i].reponse3,
                  &e.questions[i].numbr) == 5) {
        i++;
    }

    e.nombreQuestions = i;
    fclose(f);

    SDL_Surface *loadedBackground = IMG_Load("background.png");
    if (loadedBackground) {
        // Créer une surface à la taille désirée (ex: 1200x600 pour correspondre à l'écran)
        e.background = SDL_CreateRGBSurface(SDL_HWSURFACE, 1800, 1000, 32, 0, 0, 0, 0);
        
        // Redimensionner l'image chargée vers la nouvelle surface
        SDL_SoftStretch(loadedBackground, NULL, e.background, NULL);
        SDL_FreeSurface(loadedBackground);
    } else {
        fprintf(stderr, "Impossible de charger l'image de fond: %s\n", IMG_GetError());
        // Créer un fond noir par défaut
        e.background = SDL_CreateRGBSurface(SDL_HWSURFACE, 1200, 600, 32, 0, 0, 0, 0);
        SDL_FillRect(e.background, NULL, SDL_MapRGB(e.background->format, 0, 0, 50));
    }

    // Initialiser et mélanger ordreQuestions
    for (int i = 0; i < e.nombreQuestions; i++) {
        e.ordreQuestions[i] = i;
    }
    // Initialise le générateur de nombres aléatoires une fois ici
    srand(time(NULL));
    for (int i = e.nombreQuestions - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = e.ordreQuestions[i];
        e.ordreQuestions[i] = e.ordreQuestions[j];
        e.ordreQuestions[j] = temp;
    }
    e.indexActuel = 0;

    return e;
    
   
}

// Fonction pour générer une énigme aléatoire parmi celles chargées
void genererEnigmeAleatoire(Enigme *enigme) {
    if (enigme->indexActuel >= enigme->nombreQuestions) {

        return;
    }

    // Choisir la question basée sur l'ordre mélangé
    enigme->questionCourante = enigme->ordreQuestions[enigme->indexActuel];
    enigme->indexActuel++;
    enigme->etat = 0;
}

// Fonction pour initialiser les surfaces d'une énigme
void initialiserSurfaces(Enigme *enigme, TTF_Font *font, SDL_Color textColor) {

   enigme->image_EN = IMG_Load("hourglass.png");

    enigme->pos_image_EN.x = 950;
    enigme->pos_image_EN.y = 350;

    enigme->single_EN.w = 162;
    enigme->single_EN.h = 201;
    enigme->single_EN.x = 0;
    enigme->single_EN.y = 0;
    enigme->EN_num = 0;
        enigme->lastFrameTime = SDL_GetTicks();
    // Libérer les surfaces existantes et réinitialiser les pointeurs
    if (enigme->questionSurface != NULL) {

        SDL_FreeSurface(enigme->questionSurface);
        enigme->questionSurface = NULL;
    }

    if (enigme->successMessage != NULL) {
        SDL_FreeSurface(enigme->successMessage);
        enigme->successMessage = NULL;
    }

    if (enigme->failureMessage != NULL) {
        SDL_FreeSurface(enigme->failureMessage);
        enigme->failureMessage = NULL;
    }

    if (enigme->progressBar != NULL) {
        SDL_FreeSurface(enigme->progressBar);
        enigme->progressBar = NULL;
    }

    for (int i = 0; i < 3; i++) {
        if (enigme->reponseSurfaces[i] != NULL) {
            SDL_FreeSurface(enigme->reponseSurfaces[i]);
            enigme->reponseSurfaces[i] = NULL;
        }
    }

    if (enigme->questionCourante < 0 ||
        enigme->questionCourante >= enigme->nombreQuestions) {
        return;
    }

    // Créer les surfaces pour la question et les réponses
    enigme->questionSurface = TTF_RenderText_Blended(
        font, enigme->questions[enigme->questionCourante].question, textColor);
    enigme->reponseSurfaces[0] = TTF_RenderText_Blended(
        font, enigme->questions[enigme->questionCourante].reponse1, textColor);
    enigme->reponseSurfaces[1] = TTF_RenderText_Blended(
        font, enigme->questions[enigme->questionCourante].reponse2, textColor);
    enigme->reponseSurfaces[2] = TTF_RenderText_Blended(
        font, enigme->questions[enigme->questionCourante].reponse3, textColor);


    enigme->successMessage = TTF_RenderText_Blended(font, "Correct! Bien joué!",
                                                    (SDL_Color){0, 255, 0});
    enigme->failureMessage = TTF_RenderText_Blended(
        font, "Incorrect! Essayez encore.", (SDL_Color){255, 0, 0});


    enigme->questionPos.x =
        550 - enigme->questionSurface->w / 2;  // Centré horizontalement
    enigme->questionPos.y = 150;
    enigme->questionPos.w = enigme->questionSurface->w;
    enigme->questionPos.h = enigme->questionSurface->h;


    for (int i = 0; i < 3; i++) {
        enigme->reponsePos[i].x =
            400 - enigme->reponseSurfaces[i]->w / 2;  // Centré horizontalement
        enigme->reponsePos[i].y = 250 + i * 100;      // Espacées verticalement
        enigme->reponsePos[i].w = enigme->reponseSurfaces[i]->w;
        enigme->reponsePos[i].h = enigme->reponseSurfaces[i]->h;
    }


    enigme->messagePos.x = 400 - enigme->successMessage->w / 2;
    enigme->messagePos.y = 550;

    // Créer et positionner la barre de progression
    mettreAJourProgressBar(enigme, font);
}

// Fonction pour mettre à jour la barre de progression
void mettreAJourProgressBar(Enigme *enigme, TTF_Font *font) {
    char progressText[50];
    SDL_Color progressColor = {255, 215, 0};  // Couleur dorée

    if (enigme->progressBar != NULL) {
        SDL_FreeSurface(enigme->progressBar);
    }

    sprintf(progressText, "Score: %d/%d", enigme->score,
            enigme->nombreQuestions);
    enigme->progressBar =
        TTF_RenderText_Blended(font, progressText, progressColor);

    enigme->progressPos.x = 400 - enigme->progressBar->w / 2;
    enigme->progressPos.y = 50;
}

// Fonction pour afficher l'énigme courante
void afficherEnigme(SDL_Surface *ecran, Enigme *enigme, TTF_Font *font, SDL_Color textColor) {
    // Afficher l'arrière-plan s'il est chargé
    if (enigme->background) {
        SDL_BlitSurface(enigme->background, NULL, ecran, NULL);
    } else {
        // Sinon, afficher un fond noir
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 50));
    }

    // Afficher le numéro de la question au-dessus
    afficherQuestionNumero(ecran, enigme, font, textColor);

    // Afficher la barre de progression
    if (enigme->progressBar) {
        SDL_BlitSurface(enigme->progressBar, NULL, ecran, &enigme->progressPos);
    }

    // Afficher la question
    if (enigme->questionSurface != NULL) {
        SDL_BlitSurface(enigme->questionSurface, NULL, ecran, &enigme->questionPos);
    }

    // Afficher les réponses
    for (int i = 0; i < 3; i++) {
        if (enigme->reponseSurfaces[i] != NULL) {
            SDL_BlitSurface(enigme->reponseSurfaces[i], NULL, ecran, &enigme->reponsePos[i]);
        }
    }

    // Afficher les messages de succès ou d'échec
    if (enigme->etat == 1 && enigme->successMessage != NULL) {
        SDL_BlitSurface(enigme->successMessage, NULL, ecran, &enigme->messagePos);
    } else if (enigme->etat == -1 && enigme->failureMessage != NULL) {
        SDL_BlitSurface(enigme->failureMessage, NULL, ecran, &enigme->messagePos);
    }
}

// Fonction pour vérifier si un clic est sur une réponse
int verifierClicReponse(Enigme *enigme, int x, int y) {
    if (enigme->questionCourante < 0 ||
        enigme->questionCourante >= enigme->nombreQuestions) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        if (x >= enigme->reponsePos[i].x &&
            x < enigme->reponsePos[i].x + enigme->reponsePos[i].w &&
            y >= enigme->reponsePos[i].y &&
            y < enigme->reponsePos[i].y + enigme->reponsePos[i].h) {
            // Vérifier si la réponse cliquée est la bonne
            if (i + 1 == enigme->questions[enigme->questionCourante].numbr) {
                enigme->etat = 1;  // Succès
                enigme->score++;   // Mettre à jour le score
            } else {
                enigme->etat = -1;  // Échec
            }

            return i + 1;
        }
    }

    return 0;
}

// Fonction pour libérer les ressources associées à une énigme
void libererEnigme(Enigme *enigme) {
    if (enigme->questionSurface != NULL) {
        SDL_FreeSurface(enigme->questionSurface);
        enigme->questionSurface = NULL;
    }

    for (int i = 0; i < 3; i++) {
        if (enigme->reponseSurfaces[i] != NULL) {
            SDL_FreeSurface(enigme->reponseSurfaces[i]);
            enigme->reponseSurfaces[i] = NULL;
        }
    }

    if (enigme->background != NULL) {
        SDL_FreeSurface(enigme->background);
        enigme->background = NULL;
    }

    if (enigme->successMessage != NULL) {
        SDL_FreeSurface(enigme->successMessage);
        enigme->successMessage = NULL;
    }

    if (enigme->failureMessage != NULL) {
        SDL_FreeSurface(enigme->failureMessage);
        enigme->failureMessage = NULL;
    }

    if (enigme->progressBar != NULL) {
        SDL_FreeSurface(enigme->progressBar);
        enigme->progressBar = NULL;
    }
}

void afficherQuestionNumero(SDL_Surface *ecran, Enigme *enigme, TTF_Font *font, SDL_Color textColor) {
    char numeroText[50];
    // Use enigme->indexActuel to show the count; it was incremented after selecting a question.
    sprintf(numeroText, "Question %d/%d", enigme->indexActuel, enigme->nombreQuestions);
    SDL_Surface *numeroSurface = TTF_RenderText_Blended(font, numeroText, textColor);
    if (numeroSurface == NULL) {
        fprintf(stderr, "Erreur de rendu du numéro de question: %s\n", TTF_GetError());
        return;
    }
    SDL_Rect numeroPos;
    numeroPos.x = 400 - numeroSurface->w / 2;


    numeroPos.y = 100 - numeroSurface->h / 2;
    SDL_BlitSurface(numeroSurface, NULL, ecran, &numeroPos);
    SDL_FreeSurface(numeroSurface);
}
// Fonction helper pour gérer les réponses (clavier ou souris)


void gererReponse(Enigme *enigme, SDL_Surface *ecran, TTF_Font *font, SDL_Color textColor, int *done) {
    mettreAJourProgressBar(enigme, font);
    afficherEnigme(ecran, enigme, font, textColor);
    SDL_Flip(ecran);
    SDL_Delay(2000);
    
    if (enigme->indexActuel >= enigme->nombreQuestions) {
        printf("Fin de l'énigme. Score final: %d/%d\n", enigme->score, enigme->nombreQuestions);
        
        // Afficher le score final à l'écran
        char scoreText[50];
        sprintf(scoreText, "Score final: %d/%d", enigme->score, enigme->nombreQuestions);
        SDL_Surface *scoreSurface = TTF_RenderText_Blended(font, scoreText, textColor);

        SDL_Rect scorePos;
	scorePos.x = 400 - scoreSurface->w / 2;
	scorePos.y= 300 - scoreSurface->h / 2;

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 50));
        SDL_BlitSurface(scoreSurface, NULL, ecran, &scorePos);
        SDL_Flip(ecran);
        SDL_Delay(4000);
        SDL_FreeSurface(scoreSurface);
        
        *done = 0;
    } else {
        genererEnigmeAleatoire(enigme);
        initialiserSurfaces(enigme, font, textColor);
    }
}

void animerEn(Enigme *e) {

 Uint32 currentTime = SDL_GetTicks();
 if (currentTime - e->lastFrameTime > 200) 
      {
    if (e->EN_num >= 0 && e->EN_num < 5) {
        e->single_EN.x = e->EN_num * e->single_EN.w;
        e->EN_num++;
    }
    if (e->EN_num == 5) {
        e->EN_num = 0;
        e->single_EN.x = e->EN_num * e->single_EN.w;
    }
	e->lastFrameTime=currentTime;
    }
}

void afficherEn(Enigme *e, SDL_Surface *screen) {

   
      
    SDL_BlitSurface(e->image_EN, &e->single_EN, screen, &e->pos_image_EN);
   
     
}
