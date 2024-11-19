#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// Structure pour les détails des cartes (non utilisée dans ce code)
typedef struct cards {
    int nombre;
    int couleur;
    SDL_Rect rect_card;
    SDL_Texture *texture;
} cards;

// Déclarations des fonctions
SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer);
void load_background(SDL_Texture *texture, SDL_Renderer* renderer, int x, int y);
int calcul_score(int L[15], int *score);

// Tableaux pour les différentes colonnes de cartes
int L1Y[15] = {0}, L2Y[15] = {0}, L1G[15] = {0}, L2G[15] = {0}, L1W[15] = {0}, L2W[15] = {0}, L1R[15] = {0}, L2R[15] = {0}, L1B[15] = {0}, L2B[15] = {0};

// Scores pour les différents types de cartes et globaux
int score1 = 0;
int score2 = 0;
int score1y = 0, score2y = 0;
int score1g = 0, score2g = 0;
int score1w = 0, score2w = 0;
int score1r = 0, score2r = 0;
int score1b = 0, score2b = 0;
int scorepio = 44;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("lost", 100, 100, 900, 700, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur de création du rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *Back = load_texture_from_image("lostt.png", window, renderer);
    SDL_RenderClear(renderer);
    load_background(Back, renderer, 0, 0);

    TTF_Font *font = TTF_OpenFont("pl.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color textColor1 = {255, 255, 255, 255};

    int quit = 0;
    SDL_Event event;

    SDL_Texture *textTexture1 = NULL;
    SDL_Texture *textTexture2 = NULL;
    SDL_Texture *textTexture1y = NULL;
    SDL_Texture *textTexture2y = NULL;
    SDL_Texture *textTexture1g = NULL;
    SDL_Texture *textTexture2g = NULL;
    SDL_Texture *textTexture1w = NULL;
    SDL_Texture *textTexture2w = NULL;
    SDL_Texture *textTexture1r = NULL;
    SDL_Texture *textTexture2r = NULL;
    SDL_Texture *textTexture1b = NULL;
    SDL_Texture *textTexture2b = NULL;
    SDL_Texture *textTexturepio = NULL;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } 
        }
        score1y=calcul_score(L1Y, &score1y);
        score2y=calcul_score(L2Y, &score2y);
        score1g=calcul_score(L1G, &score1g);
        score2g=calcul_score(L2G, &score2g);
        score1w=calcul_score(L1W, &score1w);
        score2w=calcul_score(L2W, &score2w);
        score1r=calcul_score(L1R, &score1r);
        score2r=calcul_score(L2R, &score2r);
        score1b=calcul_score(L1B, &score1b);
        score2b=calcul_score(L2B, &score2b);

        score1 = score1y + score1g + score1w + score1r + score1b;
        score2 = score2y + score2g + score2w + score2r + score2b;


        char scoreText1[50];
        char scoreText2[50];
        char scoreText1y[50];
        char scoreText2y[50];
        char scoreText1g[50];
        char scoreText2g[50];
        char scoreText1w[50];
        char scoreText2w[50];
        char scoreText1r[50];
        char scoreText2r[50];
        char scoreText1b[50];
        char scoreText2b[50];
        char scoreTextpio[50];

        snprintf(scoreText1, sizeof(scoreText1), "%d", score1);
        snprintf(scoreText2, sizeof(scoreText2), "%d", score2);
        snprintf(scoreText1y, sizeof(scoreText1y), "%d", score1y);
        snprintf(scoreText2y, sizeof(scoreText2y), "%d", score2y);
        snprintf(scoreText1g, sizeof(scoreText1g), "%d", score1g);
        snprintf(scoreText2g, sizeof(scoreText2g), "%d", score2g);
        snprintf(scoreText1w, sizeof(scoreText1w), "%d", score1w);
        snprintf(scoreText2w, sizeof(scoreText2w), "%d", score2w);
        snprintf(scoreText1r, sizeof(scoreText1r), "%d", score1r);
        snprintf(scoreText2r, sizeof(scoreText2r), "%d", score2r);
        snprintf(scoreText1b, sizeof(scoreText1b), "%d", score1b);
        snprintf(scoreText2b, sizeof(scoreText2b), "%d", score2b);
        snprintf(scoreTextpio, sizeof(scoreTextpio), "%d", scorepio);

    
        SDL_Surface *textSurface1 = TTF_RenderText_Solid(font, scoreText1, textColor1);
        SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, scoreText2, textColor1);
        SDL_Surface *textSurface1y = TTF_RenderText_Solid(font, scoreText1y, textColor);
        SDL_Surface *textSurface2y = TTF_RenderText_Solid(font, scoreText2y, textColor);
        SDL_Surface *textSurface1g = TTF_RenderText_Solid(font, scoreText1g, textColor1);
        SDL_Surface *textSurface2g = TTF_RenderText_Solid(font, scoreText2g, textColor1);
        SDL_Surface *textSurface1w = TTF_RenderText_Solid(font, scoreText1w, textColor);
        SDL_Surface *textSurface2w = TTF_RenderText_Solid(font, scoreText2w, textColor);
        SDL_Surface *textSurface1r = TTF_RenderText_Solid(font, scoreText1r, textColor1);
        SDL_Surface *textSurface2r = TTF_RenderText_Solid(font, scoreText2r, textColor1);
        SDL_Surface *textSurface1b = TTF_RenderText_Solid(font, scoreText1b, textColor);
        SDL_Surface *textSurface2b = TTF_RenderText_Solid(font, scoreText2b, textColor);
        SDL_Surface *textSurfacepio = TTF_RenderText_Solid(font, scoreTextpio, textColor);

        if (textSurface1 == NULL || textSurface2 == NULL || textSurface1y == NULL || textSurface2y == NULL ||
            textSurface1g == NULL || textSurface2g == NULL || textSurface1w == NULL || textSurface2w == NULL ||
            textSurface1r == NULL || textSurface2r == NULL || textSurface1b == NULL || textSurface2b == NULL ||
            textSurfacepio == NULL) {
            fprintf(stderr, "Erreur de création de la surface de texte: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        if (textTexture1 != NULL) SDL_DestroyTexture(textTexture1);
        textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
        SDL_FreeSurface(textSurface1);

        if (textTexture2 != NULL) SDL_DestroyTexture(textTexture2);
        textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        SDL_FreeSurface(textSurface2);

        if (textTexture1y != NULL) SDL_DestroyTexture(textTexture1y);
        textTexture1y = SDL_CreateTextureFromSurface(renderer, textSurface1y);
        SDL_FreeSurface(textSurface1y);

        if (textTexture2y != NULL) SDL_DestroyTexture(textTexture2y);
        textTexture2y = SDL_CreateTextureFromSurface(renderer, textSurface2y);
        SDL_FreeSurface(textSurface2y);

        if (textTexture1g != NULL) SDL_DestroyTexture(textTexture1g);
        textTexture1g = SDL_CreateTextureFromSurface(renderer, textSurface1g);
        SDL_FreeSurface(textSurface1g);

        if (textTexture2g != NULL) SDL_DestroyTexture(textTexture2g);
        textTexture2g = SDL_CreateTextureFromSurface(renderer, textSurface2g);
        SDL_FreeSurface(textSurface2g);

        if (textTexture1w != NULL) SDL_DestroyTexture(textTexture1w);
        textTexture1w = SDL_CreateTextureFromSurface(renderer, textSurface1w);
        SDL_FreeSurface(textSurface1w);

        if (textTexture2w != NULL) SDL_DestroyTexture(textTexture2w);
        textTexture2w = SDL_CreateTextureFromSurface(renderer, textSurface2w);
        SDL_FreeSurface(textSurface2w);

        if (textTexture1r != NULL) SDL_DestroyTexture(textTexture1r);
        textTexture1r = SDL_CreateTextureFromSurface(renderer, textSurface1r);
        SDL_FreeSurface(textSurface1r);

        if (textTexture2r != NULL) SDL_DestroyTexture(textTexture2r);
        textTexture2r = SDL_CreateTextureFromSurface(renderer, textSurface2r);
        SDL_FreeSurface(textSurface2r);

        if (textTexture1b != NULL) SDL_DestroyTexture(textTexture1b);
        textTexture1b = SDL_CreateTextureFromSurface(renderer, textSurface1b);
        SDL_FreeSurface(textSurface1b);

        if (textTexture2b != NULL) SDL_DestroyTexture(textTexture2b);
        textTexture2b = SDL_CreateTextureFromSurface(renderer, textSurface2b);
        SDL_FreeSurface(textSurface2b);

        if (textTexturepio != NULL) SDL_DestroyTexture(textTexturepio);
        textTexturepio = SDL_CreateTextureFromSurface(renderer, textSurfacepio);
        SDL_FreeSurface(textSurfacepio);

        SDL_Rect textRect1 = { 20,10,20, 40 };
        SDL_Rect textRect2 = { 20,50,20, 40 };
        SDL_Rect textRect1y = { 50, 300, 20, 40 };
        SDL_Rect textRect2y = { 50, 350, 20, 40 };
        SDL_Rect textRect1g = { 235, 250, 20, 40 };
        SDL_Rect textRect2g = { 235, 296, 20, 40 };
        SDL_Rect textRect1w = { 418, 240, 20, 40 };
        SDL_Rect textRect2w = { 418,300, 20, 40 };
        SDL_Rect textRect1r = { 604, 246, 20, 40 };
        SDL_Rect textRect2r = { 604, 302, 20, 40 };
        SDL_Rect textRect1b = { 780, 310, 20, 40 };
        SDL_Rect textRect2b = { 780, 360, 20, 40 };
        SDL_Rect textRectpio = {825,10, 20, 40 };

        SDL_RenderClear(renderer);
        load_background(Back, renderer, 0, 0);

        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
        SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
        SDL_RenderCopy(renderer, textTexture1y, NULL, &textRect1y);
        SDL_RenderCopy(renderer, textTexture2y, NULL, &textRect2y);
        SDL_RenderCopy(renderer, textTexture1g, NULL, &textRect1g);
        SDL_RenderCopy(renderer, textTexture2g, NULL, &textRect2g);
        SDL_RenderCopy(renderer, textTexture1w, NULL, &textRect1w);
        SDL_RenderCopy(renderer, textTexture2w, NULL, &textRect2w);
        SDL_RenderCopy(renderer, textTexture1r, NULL, &textRect1r);
        SDL_RenderCopy(renderer, textTexture2r, NULL, &textRect2r);
        SDL_RenderCopy(renderer, textTexture1b, NULL, &textRect1b);
        SDL_RenderCopy(renderer, textTexture2b, NULL, &textRect2b);
        SDL_RenderCopy(renderer, textTexturepio, NULL, &textRectpio);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

// Fonction pour calculer le score pour une liste donnée de cartes
int calcul_score(cards L[], int *score) {
    int somme = 0;
    int nb_speciales = 0;

    for (int i = 0; i < 15; ++i) {
        if (L[i] >= 5 && L[i] <= 13) {
            somme += L[i];
        } else if (L[i] >= 2 && L[i] <= 4) {
            nb_speciales++;
        }
    }

    *score = (somme - 20) * (nb_speciales + 1);
    return *score;
}


SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Surface *my_image = IMG_Load(file_image_name);
    if (my_image == NULL) {
        fprintf(stderr, "Erreur: %s\n", IMG_GetError());
        return NULL;
    }
    if (window == NULL) {
        fprintf(stderr, "Erreur");
        return NULL;
    }
    SDL_Texture *my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);

    if (my_texture == NULL) {
        fprintf(stderr, "Erreur: %s\n", SDL_GetError());
        return NULL;
    }
    return my_texture;
}


void load_background(SDL_Texture *texture, SDL_Renderer* renderer, int x, int y) {
    SDL_Rect source = {0}, destination = {0};
    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
    destination.w = 900;
    destination.h = 700;
    destination.x = x;
    destination.y = y;
    SDL_RenderCopy(renderer, texture, &source, &destination);
}
