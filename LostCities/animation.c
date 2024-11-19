#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "animation.h"
#include <time.h>



int is_point_in_rect(int x, int y, SDL_Rect* rect){
    return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
}


double sigmoid(double t, double k) {
    return 1 / (1 + exp(-k * (t - 0.25)));
}


void animate_card(int x_fin, int y_fin, cards* card, SDL_Renderer* renderer) {
    static int i = 0;
    int steps = 500; 
    double k = 20;
    SDL_Rect srcRect1 = {700, 50, 100, 100};
    static double x = 700;
    static double y = 50;

    if (i <= steps) {
        double t = (double)i / steps;
        x = 700 + (x_fin - 700) * sigmoid(t, k);
        y = 50 + (y_fin - 50) * sigmoid(t, k);
        i++;
    } else {

        x = x_fin;
        y = y_fin;
    }


    card->rect_card.x = (int)x;
    card->rect_card.y = (int)y;


    SDL_RenderCopy(renderer, card->texture, &srcRect1, &card->rect_card);

    if (i > steps) {
        printf("Animation completed: x = %d, y = %d\n", (int)x, (int)y);
    }
}


