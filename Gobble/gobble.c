#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mm.h"
#include "gobble.h"









SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Surface *my_image = IMG_Load(file_image_name);
    if (my_image == NULL) {
        fprintf(stderr, "Error: %s\n", IMG_GetError());
        return NULL;
    }
    if (window == NULL){
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

void play_with_texture_2(SDL_Texture* my_texture, SDL_Renderer* renderer, int x, int y, float zoom) {
    SDL_Rect source = {0}, destination = {0};
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);

    destination.w = source.w * zoom;
    destination.h = source.h * zoom;
    destination.x = x;
    destination.y = y;

    SDL_RenderCopy(renderer, my_texture, &source, &destination);
}
void piecediplay(piece pieces[], int piece_count){
    printf("\n");
    for (int i = 0; i < piece_count; i++){
        printf("la piece %d est de couleur %d et de taille %d\n",i,pieces[i].couleur, pieces[i].taille);
    }
}
void grid_display(piece grid[3][3]){
    printf("\n");
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("(c : %d , t : %d) ",grid[i][j].couleur, grid[i][j].taille);
        }
        printf("\n");
    }
    printf("\n");
}

void handle_piece_movement(SDL_Event event, piece pieces[], int piece_count, piece grid[3][3], int *tour, SDL_Renderer *renderer, SDL_Window *window) {
    static piece* selected_piece = NULL;
    static int piece_selected = 0;
    const int square_size = 200;
    const int square_center_offset = square_size / 2;

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (!piece_selected) {
                for (int i = 0; i < piece_count; i++) {
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &pieces[i].rect_piece)) {
                        selected_piece = &pieces[i];
                        selected_piece ->texture=load_texture_from_image("gob.png" , window , renderer);
                        piece_selected = 1;
                        break;
                    }
                }
            } else  if ( x >= 150 && x <= 750 && y >= 150 && y <= 750){
                int grid_x = x / square_size;
                int grid_y = y / square_size;

                printf("%d %d\n", grid_x - 1, grid_y - 1);
                if (*tour == selected_piece->couleur){
                    if (grid[grid_y-1][grid_x - 1].couleur == 0 || grid[grid_y-1][grid_x -1].taille < selected_piece->taille){
                        
                    selected_piece->rect_piece.x = grid_x * square_size + square_center_offset - selected_piece->rect_piece.w / 2 - 55;
                    selected_piece->rect_piece.y = grid_y * square_size + square_center_offset - selected_piece->rect_piece.h / 2 - 55;
                        grid[grid_y-1][grid_x-1].couleur = selected_piece->couleur;
                        grid[grid_y-1][grid_x-1].taille = selected_piece->taille;
                        grid_display(grid);
                        
                    }
                }
                if (grid[grid_y - 1][grid_x - 1].couleur == selected_piece->couleur && grid[grid_y-1][grid_x-1].taille == selected_piece->taille){
                    if (*tour == 1){
                        (*tour)++;
                    }
                    else{
                        (*tour) = 1;
                    }
                }
                int best_col,best_row;
                    if (*tour == 2) {
                        minimax(grid, 1000, *tour, 2, &best_col, &best_row);
                        if (best_col < 3 && best_col >= 0 && best_row < 3 && best_col >= 0){
                        static int r = 17;
                        selected_piece = &pieces[r];
                        if (grid[best_col][best_row].couleur == 0 || grid[best_row][best_col].taille < selected_piece->taille) {
                            selected_piece->rect_piece.x = (best_row + 1)* square_size + square_center_offset - selected_piece->rect_piece.w / 2 - 55;
                            selected_piece->rect_piece.y = (best_col + 1) * square_size + square_center_offset - selected_piece->rect_piece.h / 2 - 55;
                            grid[best_col][best_row].couleur = selected_piece->couleur;
                            grid[best_col][best_row].taille = selected_piece->taille;
                            printf("--cordonnee_x : %d cordonee_y : %d--\n", selected_piece->rect_piece.x,selected_piece->rect_piece.y);
                        }
                        r--;
                        printf("the best move is (%d %d)\n", best_row, best_col);
                        printf("Bp color:%d Bp size: %d Sp color: %d SP size: %d\n",grid[best_row][best_col].couleur, grid[best_row][best_col].taille, selected_piece->couleur, selected_piece->taille);
                        }
                        *tour = 1;
                        grid_display(grid);
                        
                    }
                piece_selected = 0;
                selected_piece = NULL;
                
        }
    }
}}

void animations(SDL_Texture* bg_texture, SDL_Renderer* renderer, int w, int h,int ww,int hh) {
    int bgwidth, bgheight;
    SDL_QueryTexture(bg_texture, NULL, NULL, &bgwidth, &bgheight);
    static int bgoffset = 0;
    bgoffset -= 5;
    if (bgoffset <= -bgwidth) {
        bgoffset = 0;
    }
    SDL_Rect srcRect = {w, h, ww, hh};
    SDL_Rect destRect1 = {bgoffset, 0, bgwidth, bgheight};
    SDL_Rect destRect2 = {bgwidth + bgoffset, 0, bgwidth, bgheight};
    SDL_RenderCopy(renderer, bg_texture, &srcRect, &destRect1);
    SDL_RenderCopy(renderer, bg_texture, &srcRect, &destRect2);


}


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Déplacement des pièces", 100, 100, 900, 900, SDL_WINDOW_SHOWN);
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

    piece pieces[18];
    int i,tour = 1;
    for (i = 0; i < 18; i++){
        if (i < 9){
            pieces[i].texture = load_texture_from_image("bgob.png", win, renderer);
        }
        else{
            pieces[i].texture = load_texture_from_image("rgob.png", win, renderer);
        }
    }
    SDL_Texture *btext = load_texture_from_image("back.png", win, renderer);
    SDL_Rect rect = {E, E, W - 2*E, H - 2*E};
    SDL_bool running = SDL_TRUE;

    if (!pieces[0].texture || !pieces[1].texture || !pieces[2].texture || !btext) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    
    pieces[0].rect_piece = (SDL_Rect){210, 75, 60, 60};
    pieces[1].rect_piece = (SDL_Rect){220, 75, 60, 60};
    pieces[2].rect_piece = (SDL_Rect){230, 75, 60, 60};
    pieces[3].rect_piece = (SDL_Rect){410, 56, 80, 80};
    pieces[4].rect_piece = (SDL_Rect){420, 56, 80, 80};
    pieces[5].rect_piece = (SDL_Rect){430, 56, 80, 80};
    pieces[6].rect_piece = (SDL_Rect){610, 50, 90, 100};
    pieces[7].rect_piece = (SDL_Rect){620, 50, 90, 100};
    pieces[8].rect_piece = (SDL_Rect){630, 50, 90, 100};
    pieces[9].rect_piece = (SDL_Rect){210, 825, 60, 60}; //ajout de 750 offset pour le rouge
    pieces[10].rect_piece = (SDL_Rect){220, 825, 60, 60};
    pieces[11].rect_piece = (SDL_Rect){230, 825, 60, 60};
    pieces[12].rect_piece = (SDL_Rect){410, 806, 80, 80};
    pieces[13].rect_piece = (SDL_Rect){420, 806, 80, 80};
    pieces[14].rect_piece = (SDL_Rect){430, 806, 80, 80};
    pieces[15].rect_piece = (SDL_Rect){610, 800, 90, 100};
    pieces[16].rect_piece = (SDL_Rect){620, 800, 90, 100};
    pieces[17].rect_piece = (SDL_Rect){630, 800, 90, 100};
    piece grid[3][3];
    for (int i = 0 ; i < 9; i++){
        pieces[i].couleur = 1;        
        if ( i < 3){
            pieces[i].taille = 1;
        }
        else if (i < 6){
            pieces[i].taille = 2;
        }
        else{
            pieces[i].taille = 3;
        }
    }
    for (int i = 9 ; i < 18 ; i++){
        pieces[i].couleur = 2;
        if (i < 12){
            pieces[i].taille = 1;
        }
        else if (i < 15){
            pieces[i].taille = 2;
        }
        else{
            pieces[i].taille = 3;
        }
    }
    for (int i = 0 ; i < 3; i++){
        for (int j = 0; j < 3; j++){
            grid[i][j].couleur = 0;
            grid[i][j].taille = 0;
        }
    }

    SDL_Event event;
    piecediplay(pieces, 18);
    grid_display(grid);
    SDL_Texture *t=load_texture_from_image("b.png",win,renderer);
    int posx=0,posy=800;
    while (running) {


        if(grid_checker(grid) == 0){
            running = 0;
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }
            handle_piece_movement(event, pieces, 18, grid, &tour,renderer,win);
        }


        SDL_RenderCopy(renderer, btext, NULL, NULL);
        SDL_Rect rect1={800 , 100 ,30,40};
        SDL_RenderCopy(renderer,t,NULL,&rect1);
        SDL_Rect rect2={posy , posy- 900  ,30,40};
        SDL_RenderCopy(renderer,t,NULL,&rect2);
        SDL_Rect rect3={posx , 800 ,30,40};
        SDL_RenderCopy(renderer,t,NULL,&rect3);
        SDL_Rect rect4={posx , 100 ,30,40};
        SDL_RenderCopy(renderer,t,NULL,&rect4);
        SDL_Rect rect5={800 , posx ,30,40};
        SDL_RenderCopy(renderer,t,NULL,&rect5);
        posx+=15;
        posy-=22;
        if(posx>=950){
            posx=0;
        }
        if(0 >= posy){
            posy=900;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, Yun, Xun, Yun, Xde);
        SDL_RenderDrawLine(renderer, Yde, Xun, Yde, Xde);
        SDL_RenderDrawLine(renderer, Xun, Yde, Xde, Yde);
        SDL_RenderDrawLine(renderer, Xun, Yun, Xde, Yun);
        

            for (int i = 0; i < 3; i++) {
                SDL_RenderCopy(renderer, pieces[i].texture, NULL, &pieces[i].rect_piece);
                SDL_RenderCopy(renderer, pieces[i+9].texture, NULL, &pieces[i+9].rect_piece);
            }
            for (int i = 3; i < 6; i++) {
                SDL_RenderCopy(renderer, pieces[i].texture, NULL, &pieces[i].rect_piece);
                SDL_RenderCopy(renderer, pieces[i+9].texture, NULL, &pieces[i+9].rect_piece);
            }
            for (int i = 6; i < 9; i++) {
                SDL_RenderCopy(renderer, pieces[i].texture, NULL, &pieces[i].rect_piece);
                SDL_RenderCopy(renderer, pieces[i+9].texture, NULL, &pieces[i+9].rect_piece);
            }
        
        
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }


    for (int i = 0; i < 18; i++) {
        SDL_DestroyTexture(pieces[i].texture);
    }
    SDL_DestroyTexture(btext);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
    }