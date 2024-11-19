#ifndef MM_H
#define MM_H



typedef struct piece {
    int couleur; 
    int taille; 
    SDL_Texture *texture;
    SDL_Rect rect_piece;
} piece;

typedef struct joueur {
    int id;
    int g_piece;
    int m_piece;
    int s_piece;
} joueur;


typedef struct board {
    piece *cells[3][3];
} board;

int minimax(piece grid[3][3], int depth, int player_id, int maximizing_player, int *best_row, int *best_col);
int evaluate_board(piece grid[3][3], int player_id);
int grid_checker(piece grid[3][3]);

#endif