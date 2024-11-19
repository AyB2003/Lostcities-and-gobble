#ifndef GOBB_H
#define GOBB_H

#define W 900
#define H 900
#define E 150
#define Xun 150
#define Yun 350
#define Xde 750
#define Yde 550




SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer);
void play_with_texture_2(SDL_Texture* my_texture, SDL_Renderer* renderer, int x, int y, float zoom);
void piecediplay(piece pieces[], int piece_count);
void grid_display(piece grid[3][3]);


#endif