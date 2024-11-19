#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graphics.h"
#include "animation.h"
#include "mcts.h"

SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Surface *my_image = IMG_Load(file_image_name);
    if (my_image == NULL) {
        fprintf(stderr, "Error: %s\n", IMG_GetError());
        return NULL;
    }
    if (window == NULL) {
        fprintf(stderr, "Error");
        return NULL;
    }

    SDL_Texture *my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);

    if (my_texture == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return NULL;
    }

    return my_texture;
}

cards draw_next_card_from_deck1(cards allcards[], int a){
    return allcards[a];
}

void swap(cards *a, cards *b) {
    cards temp = *a;
    *a = *b;
    *b = temp;
}


void shuffle(cards *deck, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&deck[i], &deck[j]);
    }
}

void display_played_cards(cards card[], int size){
    printf("\n");
    if (size == 0){
        printf ("Rien du tout\n");
    }
    for (int i = 0; i < size; i++){
        printf("(c: %d;m: %d)\n", card[i].couleur, card[i].nombre - 3);
    }
    printf("\n");
}

int compare_cards(const void* a, const void* b) {
    cards* cardA = (cards*)a;
    cards* cardB = (cards*)b;

    if (cardA->couleur < cardB->couleur) {
        return -1;
    } else if (cardA->couleur > cardB->couleur) {
        return 1;
    } else {
        if (cardA->nombre < cardB->nombre) {
            return -1;
        } else if (cardA->nombre > cardB->nombre) {
            return 1;
        } else {
            return 0;
        }
    }
}

void load_background(SDL_Texture *texture, SDL_Renderer* renderer, int x, int y){
    SDL_Rect source = {0}, destination = {0};
    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
    destination.w = 900;
    destination.h = 700;
    destination.x = x;
    destination.y = y;
    SDL_RenderCopy(renderer, texture, &source, &destination);
}

void calcul_score(cards L[], int *score, int size) {
    int somme = 0;
    int nb_speciales = 0;

    for (int i = 0; i < size; ++i) {
        if (L[i].nombre >= 5 && L[i].nombre <= 13) {
            somme += L[i].nombre - 3;
        } else if (L[i].nombre >= 2 && L[i].nombre <= 4) {
            nb_speciales++;
        }
    }

    *score = (somme - 20) * (nb_speciales + 1);
}


int in(int tab[], int v, int size){
    for (int i = 0 ; i < size; i++){
        if (v == tab[i]){
            return 1;
        }
    }
    return 0;
}


int present(cards Deck[], int size , cards* card){
    for(int i=0 ; i < size; i++){
        if(card->couleur == Deck[i].couleur && card->nombre==Deck[i].nombre) return 1;
    }
    return 0;
}


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Lost Cities", 100, 100, 900, 700, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_bool running = SDL_TRUE;
    SDL_Event event;
    cards back[52];
    cards allcards[60];
    cards selected_cards[8];
    cards selected_cards1[8];
    cards* PlayedY1 = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedG1 = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedW1 = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedR1 = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedB1 = (cards*)malloc(sizeof(cards)*12);

    cards* PlayedY = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedG = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedW = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedR = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedB = (cards*)malloc(sizeof(cards)*12);

    cards* PlayedYy = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedGg = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedWw = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedRr = (cards*)malloc(sizeof(cards)*12);
    cards* PlayedBb = (cards*)malloc(sizeof(cards)*12);

    cards* selected_card = NULL;
    cards* newcard = NULL;

    GameState* state = (GameState*)malloc(sizeof(GameState));
    state->deck_size = 60;
    
    SDL_Rect deckpos = {700, 50, 64, 64};
    SDL_Rect YRect = {10, 500, 50, 50};
    SDL_Rect GRect = {10 + 180, 500, 50, 50};
    SDL_Rect WRect = {10 + 180*2, 500, 50, 50};
    SDL_Rect RRect = {10 + 180*3, 500, 50, 50};
    SDL_Rect BRect = {10 + 180*4, 500, 50, 50};
    SDL_Rect YRect1 = {10, 200, 50, 50};
    SDL_Rect GRect1 = {10 + 180, 200, 50, 50};
    SDL_Rect WRect1 = {10 + 180*2, 200, 50, 50};
    SDL_Rect RRect1 = {10 + 180*3, 200, 50, 50};
    SDL_Rect BRect1 = {10 + 180*4, 200, 50, 50};
    SDL_Rect YRectt = {10, 350, 50, 50};
    SDL_Rect GRectt = {10 + 180, 350, 50, 50};
    SDL_Rect WRectt = {10 + 180*2, 350, 50, 50};
    SDL_Rect RRectt = {10 + 180*3, 350, 50, 50};
    SDL_Rect BRectt = {10 + 180*4, 350, 50, 50};
    char path[50];
    int i = 0;
    int j = 8;
    int current_card = 0;
    int p = 0;
    int n=0;
    int id_playedY = 0;
    int id_playedG = 0;
    int id_playedW = 0;
    int id_playedR = 0;
    int id_playedB = 0;
    int id_playedY1 = 0;
    int id_playedG1 = 0;
    int id_playedW1 = 0;
    int id_playedR1 = 0;
    int id_playedB1 = 0;
    int  d = 0;
    int c = 0;
    int scoreY = 0;
    int scoreG = 0;
    int scoreW = 0;
    int scoreR = 0;
    int scoreB = 0;
    int scoretot = 0;
    int a = 10;
    int temp;
    int sx,sy;
    int turn = 1;


    PlayedG[0].nombre = 0;
    PlayedG[0].couleur = 2;
    PlayedY[0].nombre = 0;
    PlayedY[0].couleur = 1;
    PlayedR[0].nombre = 0;
    PlayedR[0].couleur = 4;
    PlayedW[0].nombre = 0;
    PlayedW[0].couleur = 3;
    PlayedB[0].nombre = 0;
    PlayedB[0].couleur = 5;

    PlayedGg[0].nombre = 0;
    PlayedGg[0].couleur = 2;
    PlayedYy[0].nombre = 0;
    PlayedYy[0].couleur = 1;
    PlayedRr[0].nombre = 0;
    PlayedRr[0].couleur = 4;
    PlayedWw[0].nombre = 0;
    PlayedWw[0].couleur = 3;
    PlayedBb[0].nombre = 0;
    PlayedBb[0].couleur = 5;

    int id_playedYy = 0;
    int id_playedGg = 0;
    int id_playedWw = 0;
    int id_playedRr = 0;
    int id_playedBb = 0;

    
    

    SDL_Texture *bg_texture = load_texture_from_image("lost.png", win, renderer);
    SDL_Texture *Highlight = load_texture_from_image("Highlight.png", win, renderer);
    SDL_Texture *Back = load_texture_from_image("back.png", win, renderer);
    SDL_Texture *Ytexture =load_texture_from_image("ys.png", win, renderer);
    SDL_Texture *Rtexture =load_texture_from_image("rs.png", win, renderer);
    SDL_Texture *Gtexture =load_texture_from_image("gs.png", win, renderer);
    SDL_Texture *Wtexture =load_texture_from_image("ws.png", win, renderer);
    SDL_Texture *Btexture =load_texture_from_image("bs.png", win, renderer);

    for (int i = 0; i < 52; i++) {
        back[i].couleur = 0;
        back[i].nombre = 0;
        back[i].texture = Back;
        SDL_Rect reack = {700, 50, 41, 41}; // Tous les back initiaux � (700, 50)
        back[i].rect_card = reack;
    }
    for (int i = 0; i < 60; i++) {
        if (i < 12) {
            allcards[i].couleur = 1;
            allcards[i].nombre = i + 2;
            sprintf(path, "Numbers/Yellow/%dy.png", i + 2);
        } else if (i < 24) {
            allcards[i].couleur = 2;
            allcards[i].nombre = i  + 2 - 12;
            sprintf(path, "Numbers/Green/%dg.png", i + 2 - 12);
        } else if (i < 36) {
            allcards[i].couleur = 3;
            allcards[i].nombre = i  + 2 - 24;
            sprintf(path, "Numbers/White/%dw.png", i + 2 - 24);
        } else if (i < 48) {
            allcards[i].couleur = 4;
            allcards[i].nombre = i  + 2 - 36;
            sprintf(path, "Numbers/REd/%dr.png", i + 2 - 36);
        } else {
            allcards[i].couleur = 5;
            allcards[i].nombre = i  + 2 - 48;
            sprintf(path, "Numbers/Blue/%db.png", i + 2 - 48);
        }
        allcards[i].texture = load_texture_from_image(path, win, renderer);
    }
    shuffle(allcards ,60);
    while (i < 8) {
        selected_cards[i].texture = allcards[i].texture;
        selected_cards[i].nombre = allcards[i].nombre;
        selected_cards[i].couleur = allcards[i].couleur;
        i++;
    }
    for (int j = 0; j < 8; j++){
        selected_cards1[j].texture = allcards[j + 8].texture;
        selected_cards1[j].nombre = allcards[j + 8].nombre;
        selected_cards1[j].couleur = allcards[j + 8].couleur;
    }
    qsort(selected_cards, 8, sizeof(cards), compare_cards);

    for (int i = 0; i < 8; i++) {
        SDL_Rect Rect = {134 + i * 85, 626, 41, 41};
        selected_cards[i].rect_card = Rect;
    }
    n=0;
    p=0;
   while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                if (turn == 1){

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouse_x = event.button.x;
                        int mouse_y = event.button.y;
                        if (newcard == NULL && selected_card == NULL && p > n ){
                            if (is_point_in_rect(mouse_x, mouse_y, &deckpos) && p > n){
                                newcard = &back[j];
                                selected_cards[temp] = draw_next_card_from_deck1(allcards, a);
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                state->deck_size--;
                                printf("--%d %d %d--\n", back[j].rect_card.x, back[j].rect_card.y,j);
                                j++;
                                n++;
                                a++;
                                newcard = NULL;
                            }
                            if (is_point_in_rect(mouse_x, mouse_y, &GRectt) && p > n && id_playedGg > 0){
                                selected_cards[temp].texture = PlayedGg[id_playedGg - 1].texture;
                                selected_cards[temp].couleur = 1;
                                selected_cards[temp].nombre = PlayedGg[id_playedGg - 1].nombre;
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                id_playedGg--;
                                n++;
                            }
                            else if (is_point_in_rect(mouse_x, mouse_y, &WRectt) && p > n && id_playedWw > 0){
                                selected_cards[temp].texture = PlayedYy[id_playedWw - 1].texture;
                                selected_cards[temp].couleur = 1;
                                selected_cards[temp].nombre = PlayedYy[id_playedWw - 1].nombre;
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                id_playedWw--;
                                n++;
                            }
                            else if (is_point_in_rect(mouse_x, mouse_y, &RRectt) && p > n && id_playedRr > 0){
                                selected_cards[temp].texture = PlayedRr[id_playedRr - 1].texture;
                                selected_cards[temp].couleur = 1;
                                selected_cards[temp].nombre = PlayedRr[id_playedRr - 1].nombre;
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                id_playedRr--;
                                n++;
                            }
                            else if (is_point_in_rect(mouse_x, mouse_y, &BRectt) && p > n && id_playedBb > 0){
                                selected_cards[temp].texture = PlayedBb[id_playedBb - 1].texture;
                                selected_cards[temp].couleur = 1;
                                selected_cards[temp].nombre = PlayedBb[id_playedBb - 1].nombre;
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                id_playedBb--;
                                n++;
                            }
                            else if (is_point_in_rect(mouse_x, mouse_y, &YRectt) && p > n && id_playedYy > 0){
                                selected_cards[temp].texture = PlayedYy[id_playedBb - 1].texture;
                                selected_cards[temp].couleur = 1;
                                selected_cards[temp].nombre = PlayedYy[id_playedBb - 1].nombre;
                                selected_cards[temp].rect_card.x = sx;
                                selected_cards[temp].rect_card.y = sy;
                                selected_cards[temp].rect_card.h = 41;
                                selected_cards[temp].rect_card.w = 41;
                                id_playedYy--;
                                n++;
                            }
                            
                        } 
                        if (selected_card == NULL ) {
                            for (int i = 0; i < 8; i++) {
                                if (is_point_in_rect(mouse_x, mouse_y, &selected_cards[i].rect_card) && p==n) {
                                    selected_card = &selected_cards[i];
                                    sx = selected_card->rect_card.x;
                                    sy = selected_card->rect_card.y;
                                    d = selected_card->nombre;
                                    temp = i;
                                    if (selected_card->couleur == 1){
                                        c = 1;
                                    }
                                    else if (selected_card->couleur == 2){
                                        c = 2;
                                    }
                                    else if (selected_card->couleur == 3){
                                        c = 3;
                                    }
                                    else if (selected_card->couleur == 4){
                                        c = 4;
                                    }
                                    else if (selected_card->couleur == 5){
                                        c = 5;
                                    }
                                    break;
                                }
                            }
                        } else {
                            SDL_bool card_found = SDL_FALSE;
                            for (int i = 0; i < 8; i++) {
                                if (is_point_in_rect(mouse_x, mouse_y, &selected_cards[i].rect_card)) {
                                    selected_card = NULL;
                                    card_found = SDL_TRUE;
                                    break;
                                }
                            }

                            if (!card_found) {
                                if ( c==1 && ( is_point_in_rect(mouse_x, mouse_y, &YRect) || is_point_in_rect(mouse_x, mouse_y, &YRectt))){
                                    if(is_point_in_rect(mouse_x, mouse_y, &YRect) &&  ( c == 1 && PlayedY[id_playedY].nombre < d) && (!present(PlayedYy ,20, selected_card) && !present(PlayedY, 12, selected_card))){
                                        selected_card->rect_card.x = YRect.x;
                                        selected_card->rect_card.y = YRect.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;
                                        
                                        PlayedY[id_playedY].nombre = selected_card->nombre;
                                        PlayedY[id_playedY].couleur = selected_card->couleur;
                                        PlayedY[id_playedY].texture = selected_card->texture;
                                        PlayedY[id_playedY].rect_card = selected_card->rect_card;
                                        
                                        c = 0;
                                        YRect.x += 20;
                                        YRect.y -= 20;
                                        calcul_score(PlayedY,&scoreY, id_playedY);
                                        id_playedY++;
                                        PlayedY[id_playedY].nombre=PlayedY[id_playedY - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    if(is_point_in_rect(mouse_x, mouse_y, &YRectt) && (!present(PlayedY, 20, selected_card) && !present(PlayedYy, 12, selected_card))){
                                        selected_card->rect_card.x = YRectt.x;
                                        selected_card->rect_card.y = YRectt.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;
                                        
                                        PlayedYy[id_playedYy].nombre = selected_card->nombre;
                                        PlayedYy[id_playedYy].couleur = selected_card->couleur;
                                        PlayedYy[id_playedYy].texture = selected_card->texture;
                                        PlayedYy[id_playedYy].rect_card = selected_card->rect_card;
                                        
                                        c = 0;
                                        YRectt.x += 5;
                                        YRectt.y -= 5;
                                        
                                        id_playedYy++;
                                        PlayedYy[id_playedYy].nombre=PlayedYy[id_playedYy - 1].nombre;
                                        p = p+1;
                                        turn = 2;

                                    }
                                    
                                    
                                }
                                else if ( c == 2 &&(is_point_in_rect(mouse_x, mouse_y, &GRect) || is_point_in_rect(mouse_x, mouse_y, &GRectt )) &&  p==n){
                                    if( PlayedG[id_playedG].nombre  < d  && is_point_in_rect(mouse_x , mouse_y , &GRect) && (!present(PlayedG, 20, selected_card) && !present(PlayedGg, 12, selected_card))){
                                        selected_card->rect_card.x = GRect.x;
                                        selected_card->rect_card.y = GRect.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedG[id_playedG].nombre = selected_card->nombre;
                                        PlayedG[id_playedG].couleur = selected_card->couleur;
                                        PlayedG[id_playedG].texture = selected_card->texture;
                                        PlayedG[id_playedG].rect_card = selected_card->rect_card;

                                        c = 0;
                                        GRect.x += 20;
                                        
                                        GRect.y -= 20;
                                        id_playedG++;
                                        calcul_score(PlayedG,&scoreG, id_playedG);
                                        
                                        PlayedG[id_playedG].nombre=PlayedG[id_playedG - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    if(is_point_in_rect(mouse_x , mouse_y , &GRectt) && (!present(PlayedG, 20, selected_card) && !present(PlayedGg, 12, selected_card))){
                                            selected_card->rect_card.x = GRectt.x;
                                            selected_card->rect_card.y = GRectt.y;
                                            selected_card->rect_card.w = 40;
                                            selected_card->rect_card.h = 40;

                                            PlayedGg[id_playedGg].nombre = selected_card->nombre;
                                            PlayedGg[id_playedGg].couleur = selected_card->couleur;
                                            PlayedGg[id_playedGg].texture = selected_card->texture;
                                            PlayedGg[id_playedGg].rect_card = selected_card->rect_card;

                                            c = 0;
                                            GRectt.x += 5;
                                            GRectt.y -= 5;
                                            id_playedGg++;
                                            PlayedGg[id_playedGg].nombre=PlayedGg[id_playedGg - 1].nombre;
                                            p = p+1;
                                            turn = 2;
                                    }
                                    
                                }
                                else if ( c == 3 &&(is_point_in_rect(mouse_x, mouse_y, &WRect) || is_point_in_rect(mouse_x , mouse_y , &WRectt) )&&  p==n){
                                    if( PlayedW[id_playedW].nombre < d && is_point_in_rect( mouse_x , mouse_y , &WRect) && (!present(PlayedW, 20, selected_card) && !present(PlayedWw, 12, selected_card))){
                                        selected_card->rect_card.x = WRect.x;
                                        selected_card->rect_card.y = WRect.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedW[id_playedW].nombre = selected_card->nombre;
                                        PlayedW[id_playedW].couleur = selected_card->couleur;
                                        PlayedW[id_playedW].texture = selected_card->texture;
                                        PlayedW[id_playedW].rect_card = selected_card->rect_card;

                                        c = 0;
                                        WRect.x += 20;
                                        WRect.y -= 20;
                                        id_playedW++;
                                        calcul_score(PlayedW,&scoreW, id_playedW);
                                        
                                        PlayedW[id_playedW].nombre=PlayedW[id_playedW - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    if(is_point_in_rect( mouse_x , mouse_y , &WRectt) && (!present(PlayedW, 20, selected_card) && !present(PlayedWw, 12, selected_card))){
                                        selected_card->rect_card.x = WRectt.x;
                                        selected_card->rect_card.y = WRectt.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedWw[id_playedWw].nombre = selected_card->nombre;
                                        PlayedWw[id_playedWw].couleur = selected_card->couleur;
                                        PlayedWw[id_playedWw].texture = selected_card->texture;
                                        PlayedWw[id_playedWw].rect_card = selected_card->rect_card;

                                        c = 0;
                                        RRectt.x += 5;
                                        RRectt.y -= 5;
                                        id_playedWw++;
                                        PlayedWw[id_playedWw].nombre=PlayedWw[id_playedWw - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    
                                }
                                else if ( c == 5 && (is_point_in_rect(mouse_x, mouse_y, &BRect) || is_point_in_rect(mouse_x , mouse_y , &BRectt) )&&  p==n){
                                    if(PlayedB[id_playedB].nombre < d && is_point_in_rect( mouse_x , mouse_y , &BRect) && (!present(PlayedB, 20, selected_card) && !present(PlayedBb, 12, selected_card))){
                                        selected_card->rect_card.x = BRect.x;
                                        selected_card->rect_card.y = BRect.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedB[id_playedB].nombre = selected_card->nombre;
                                        PlayedB[id_playedB].couleur = selected_card->couleur;
                                        PlayedB[id_playedB].texture = selected_card->texture;
                                        PlayedB[id_playedB].rect_card = selected_card->rect_card;

                                        c = 0;
                                        BRect.x += 20;
                                        BRect.y -= 20;
                                        id_playedB++;
                                        calcul_score(PlayedB,&scoreB, id_playedB);
                                        
                                        PlayedB[id_playedB].nombre=PlayedB[id_playedB - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    if(is_point_in_rect( mouse_x , mouse_y , &BRectt) && (!present(PlayedB, 20, selected_card) && !present(PlayedBb, 12, selected_card))){
                                        selected_card->rect_card.x = BRectt.x;
                                        selected_card->rect_card.y = BRectt.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedBb[id_playedBb].nombre = selected_card->nombre;
                                        PlayedBb[id_playedBb].couleur = selected_card->couleur;
                                        PlayedBb[id_playedBb].texture = selected_card->texture;
                                        PlayedBb[id_playedBb].rect_card = selected_card->rect_card;

                                        c = 0;
                                        BRectt.x += 5;
                                        BRectt.y -= 5;
                                        id_playedBb++;
                                        PlayedBb[id_playedBb].nombre=PlayedBb[id_playedBb - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    
                                }
                                else if ( c == 4 &&(is_point_in_rect(mouse_x, mouse_y, &RRect) || is_point_in_rect(mouse_x , mouse_y , &RRectt) )&&  p==n){
                                    if( PlayedR[id_playedR].nombre < d && is_point_in_rect( mouse_x , mouse_y , &RRect) && (!present(PlayedR, 20, selected_card) && !present(PlayedRr, 12, selected_card))){
                                        selected_card->rect_card.x = RRect.x;
                                        selected_card->rect_card.y = RRect.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedR[id_playedR].nombre = selected_card->nombre;
                                        PlayedR[id_playedR].couleur = selected_card->couleur;
                                        PlayedR[id_playedR].texture = selected_card->texture;
                                        PlayedR[id_playedR].rect_card = selected_card->rect_card;

                                        c = 0;
                                        RRect.x += 20;
                                        RRect.y -= 20;
                                        id_playedR++;
                                        calcul_score(PlayedR,&scoreR, id_playedR);
                                        
                                        PlayedR[id_playedR].nombre=PlayedR[id_playedR - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    if(is_point_in_rect( mouse_x , mouse_y , &RRectt) && (!present(PlayedR, 20, selected_card) && !present(PlayedRr, 12, selected_card))){
                                        selected_card->rect_card.x = RRectt.x;
                                        selected_card->rect_card.y = RRectt.y;
                                        selected_card->rect_card.w = 40;
                                        selected_card->rect_card.h = 40;

                                        PlayedRr[id_playedRr].nombre = selected_card->nombre;
                                        PlayedRr[id_playedRr].couleur = selected_card->couleur;
                                        PlayedRr[id_playedRr].texture = selected_card->texture;
                                        PlayedRr[id_playedRr].rect_card = selected_card->rect_card;

                                        c = 0;
                                        RRectt.x += 5;
                                        RRectt.y -= 5;
                                        id_playedRr++;
                                        PlayedRr[id_playedRr].nombre=PlayedRr[id_playedRr - 1].nombre;
                                        p = p+1;
                                        turn = 2;
                                    }
                                    
                                }
                                else{
                                    selected_card = NULL;
                                }
                                scoretot = scoreB + scoreY + scoreG + scoreR + scoreW;
                                display_played_cards(PlayedY,id_playedY);
                                display_played_cards(PlayedYy, id_playedYy);
                                printf("%d\n", id_playedYy);
                                display_played_cards(PlayedG,id_playedG);
                                display_played_cards(PlayedW,id_playedW);
                                display_played_cards(PlayedR,id_playedR);
                                display_played_cards(PlayedB,id_playedB);
                                printf(" Y : %d/ G : %d/ W: %d/ R: %d/ B: %d/ Tot: %d\n", scoreY, scoreG, scoreW, scoreR, scoreB, scoretot);
                                
                            }
                        }

                    }
                    break;
                }
                if (turn == 2){
                   MCTSNode* bestmove = monte_carlo_tree_search(*state, 1);
                    
                    printf("His move: \n");
                    selected_cards1[0] = draw_next_card_from_deck1(allcards, a);
                    state=&bestmove->state;
                    
                    turn = 1;
                    a++;
                    printf("My cards: \n");
                    display_played_cards(selected_cards, 8);
                    printf("His cards: \n");
                    display_played_cards(selected_cards1, 8);


                }
            }
        
        }

        SDL_RenderClear(renderer);
        load_background(bg_texture, renderer, 0, 0);
        //Gestion des emplacements des cartes
        if (current_card < 8) {
            animate_card(134 + current_card * 85, 626, &back[current_card], renderer);
            if (abs(back[current_card].rect_card.x - 134 - current_card * 85) <= 5 &&
                abs(back[current_card].rect_card.y - 626) <= 5) {
                    back[current_card].rect_card.x = 134 + current_card*85;
                    back[current_card].rect_card.y = 626;
                current_card++;
            }
        }
        for (int i = 0; i < id_playedY; i++){
            SDL_RenderCopy(renderer, PlayedY[i].texture, NULL , &PlayedY[i].rect_card);
        }
        for (int i = 0; i < id_playedYy; i++){
            SDL_RenderCopy(renderer, PlayedYy[i].texture, NULL, &PlayedYy[i].rect_card);
        }
        for (int i = 0; i < id_playedG; i++){
            SDL_RenderCopy(renderer, PlayedG[i].texture, NULL , &PlayedG[i].rect_card);
        }
        for (int i = 0; i < id_playedGg; i++){
            SDL_RenderCopy(renderer, PlayedGg[i].texture, NULL, &PlayedGg[i].rect_card);
        }
        for (int i = 0; i < id_playedW; i++){
            SDL_RenderCopy(renderer, PlayedW[i].texture, NULL , &PlayedW[i].rect_card);
        }
        for (int i = 0; i < id_playedWw; i++){
            SDL_RenderCopy(renderer, PlayedWw[i].texture, NULL, &PlayedWw[i].rect_card);
        }
        for (int i = 0; i < id_playedR; i++){
            SDL_RenderCopy(renderer, PlayedR[i].texture, NULL , &PlayedR[i].rect_card);
        }
        for (int i = 0; i < id_playedRr; i++){
            SDL_RenderCopy(renderer, PlayedRr[i].texture, NULL, &PlayedRr[i].rect_card);
        }
        for (int i = 0; i < id_playedB; i++){
            SDL_RenderCopy(renderer, PlayedB[i].texture, NULL , &PlayedB[i].rect_card);
        }
        for (int i = 0; i < id_playedBb; i++){
            SDL_RenderCopy(renderer, PlayedBb[i].texture, NULL, &PlayedBb[i].rect_card);
        }
        for (int i = 0; i < 52; i++) {
            SDL_RenderCopy(renderer, back[i].texture, NULL, &back[i].rect_card);
        }
        for (int k = 0; k < current_card; k++){
            SDL_RenderCopy(renderer, selected_cards[k].texture, NULL, &selected_cards[k].rect_card);
        }
        if (c == 1){
            SDL_RenderCopy(renderer, Ytexture, NULL, &YRect);
            SDL_RenderCopy(renderer, Ytexture, NULL, &YRectt);
        }
        else if (c == 2){
            SDL_RenderCopy(renderer, Gtexture, NULL, &GRect);
            SDL_RenderCopy(renderer, Gtexture, NULL, &GRectt);
        }
        else if (c == 3){
            SDL_RenderCopy(renderer, Wtexture, NULL, &WRect);
            SDL_RenderCopy(renderer, Wtexture, NULL, &WRectt);
        }
        else if (c == 4){
            SDL_RenderCopy(renderer, Rtexture, NULL, &RRect);
            SDL_RenderCopy(renderer, Rtexture, NULL, &RRectt);
        }
        else if (c == 5){
            SDL_RenderCopy(renderer, Btexture, NULL, &BRect);
            SDL_RenderCopy(renderer, Btexture, NULL, &BRectt);
        }
        for(int i=0; i<5; i++){
            for(int j=0; j<12; j++){
                if(state->expedition1[i][j].couleur != 0 && state->expedition1[i][j].nombre != 0){
                    if(i== 0){
                        SDL_RenderCopy(renderer,allcards[j + 8].texture,NULL,&YRect1);
                    }
                    if (i == 1){
                        SDL_RenderCopy(renderer,allcards[j + 8].texture,NULL, &GRect1);
                    }
                    if (i == 2){
                        SDL_RenderCopy(renderer,allcards[j + 8].texture,NULL, &WRect1);
                    }
                    if (i == 3){
                        SDL_RenderCopy(renderer,allcards[j + 8].texture,NULL, &RRect1);
                    }
                    if (i == 4){
                        SDL_RenderCopy(renderer,allcards[j + 8].texture,NULL, &BRect1);
                    }
                }
            }
        }
        for (int i = 0; i < 8; i++){
            state->hand[i] = selected_cards1[i];
        }
        for (int i = 0; i < 12; i++){
            state->top[0] = id_playedY;
            state->top_discard[0] = id_playedYy;
            if (i < id_playedY){
                state->expedition2[0][i] = PlayedY[i];
            }
            else{
                state->expedition2[0][i].couleur = 0;
                state->expedition2[0][i].nombre = 0;
            }
            if (i < id_playedYy){
                state->discard[0][i] = PlayedYy[i];
            }
            else{
                state->discard[0][i].couleur = 0;
                state->discard[0][i].nombre = 0;
            }

        }
        for (int i = 0; i < 12; i++){
            if (i < id_playedY1){
                state->expedition1[0][i] = PlayedY1[i];
            }
            else{
                state->expedition1[0][i].couleur = 0;
                state->expedition1[0][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            state->top[1] = id_playedG;
            state->top_discard[1] = id_playedGg;
            if (i < id_playedG){
                state->expedition2[1][i] = PlayedG[i];
            }
            else{
                state->expedition2[1][i].couleur = 0;
                state->expedition2[1][i].nombre = 0;
            }
            if (i < id_playedGg){
                state->discard[1][i] = PlayedGg[i];
            }
            else{
                state->discard[1][i].couleur = 0;
                state->discard[1][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            if (i < id_playedG1){
                state->expedition1[1][i] = PlayedG1[i];
            }
            else{
                state->expedition1[1][i].couleur = 0;
                state->expedition1[1][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            state->top[2] = id_playedW;
            state->top_discard[2] = id_playedWw;
            if (i < id_playedW){
                state->expedition2[2][i] = PlayedW[i];
            }
            else{
                state->expedition2[2][i].couleur = 0;
                state->expedition2[2][i].nombre = 0;
            }
            if (i < id_playedWw){
                state->discard[2][i] = PlayedWw[i];
            }
            else{
                state->discard[2][i].couleur = 0;
                state->discard[2][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            if (i < id_playedW1){
                state->expedition1[2][i] = PlayedW1[i];
            }
            else{
                state->expedition1[2][i].couleur = 0;
                state->expedition1[2][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            state->top[3] = id_playedR;
            state->top_discard[3] = id_playedRr;
            if (i < id_playedR){
                state->expedition2[3][i] = PlayedR[i];
            }
            else{
                state->expedition2[3][i].couleur = 0;
                state->expedition2[3][i].nombre = 0;
            }
            if (i < id_playedRr){
                state->discard[3][i] = PlayedRr[i];
            }
            else{
                state->discard[3][i].couleur = 0;
                state->discard[3][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            if (i < id_playedR1){
                state->expedition1[3][i] = PlayedR1[i];
            }
            else{
                state->expedition1[3][i].couleur = 0;
                state->expedition1[3][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            state->top[4] = id_playedB;
            state->top_discard[0] = id_playedBb;
            if (i < id_playedB){
                state->expedition2[4][i] = PlayedB[i];
            }
            else{
                state->expedition2[4][i].couleur = 0;
                state->expedition2[4][i].nombre = 0;
            }
            if (i < id_playedBb){
                state->discard[4][i] = PlayedBb[i];
            }
            else{
                state->discard[4][i].couleur = 0;
                state->discard[4][i].nombre = 0;
            }
        }
        for (int i = 0; i < 12; i++){
            if (i < id_playedB1){
                state->expedition1[4][i] = PlayedB1[i];
            }
            else{
                state->expedition1[4][i].couleur = 0;
                state->expedition1[4][i].nombre = 0;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    for (int i = 0; i < 60; i++) {
        SDL_DestroyTexture(allcards[i].texture);
    }
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyTexture(Highlight);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}