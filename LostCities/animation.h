#ifndef AN_H
#define AN_H

typedef struct cards{
    int nombre;
    int couleur;
    SDL_Rect rect_card;
    SDL_Texture *texture;
} cards;


void animate_card(int x_fin, int y_fin, cards* card, SDL_Renderer* renderer);
int is_point_in_rect(int x, int y, SDL_Rect* rect);
void handle_card_selection_and_placement(cards* card, SDL_Renderer* renderer);

#endif