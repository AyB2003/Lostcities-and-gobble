#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <limits.h>
#include "mm.h"



int evaluate_board(piece grid[3][3], int player_id) {
    int score = 0;
    int i,j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i < 2 && grid[i][j].couleur == grid[i+1][j].couleur) {
                if (player_id == 2){
                    score += 40;
                }
                else{
                    score -= 1000;
                }
                
            }
            if (j < 2 && grid[i][j].couleur == grid[i][j+1].couleur)  {
                if (player_id == 2){
                    score += 40;
                }
                else{
                    score -= 1000;
                }
            }
            if (i < 2 && j < 2 && grid[i][j].couleur == grid[i+1][j+1].couleur) {
                if (player_id == 2){
                    score += 40;
                }
                else{
                    score -= 1000;
                } 
            }
            if (i < 2 && j > 0 && grid[i][j].couleur == grid[i+1][j-1].couleur) {
                if (player_id == 2){
                    score += 40;
                }
                else{
                    score -= 1000;
                }
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (grid[i][0].couleur == player_id && grid[i][1].couleur == player_id && grid[i][2].couleur == player_id) {
            if (player_id == 2){
                score += 10000;
            }
            else{
                score -= 10000;
            }
            
        }
    }
    

    for (int j = 0; j < 3; ++j) {
        if (grid[0][j].couleur == player_id && grid[1][j].couleur == player_id && grid[2][j].couleur == player_id) {
            if (player_id == 2){
                score += 10000;
            }
            else{
                score -= 10000;
                }
    
    
    if (grid[0][0].couleur == player_id && grid[1][1].couleur == player_id && grid[2][2].couleur == player_id) {
            if (player_id == 2){
                score += 10000;
            }
            else{
                score -= 10000;
            }
    }
    if (grid[0][2].couleur == player_id && grid[1][1].couleur == player_id && grid[2][0].couleur == player_id) {
            if (player_id == 2){
                score += 10000;
            }
            else{
                score -= 10000;
            }
    }}}
    
    return score;}
int grid_checker(piece grid[3][3]) {
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i][j].couleur == 0) {
                k += 0;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (grid[i][0].couleur != 0 && grid[i][0].couleur == grid[i][1].couleur && grid[i][1].couleur == grid[i][2].couleur) {
            k += 1;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (grid[0][j].couleur != 0 && grid[0][j].couleur == grid[1][j].couleur && grid[1][j].couleur == grid[2][j].couleur) {
            k += 1;
        }
    }

    if (grid[0][0].couleur != 0 && grid[0][0].couleur == grid[1][1].couleur && grid[1][1].couleur == grid[2][2].couleur) {
        k += 1;
    }

    if (grid[0][2].couleur != 0 && grid[0][2].couleur == grid[1][1].couleur && grid[1][1].couleur == grid[2][0].couleur) {
        k += 1;
    }

    k += -1;
    return k;
}


int minimax(piece grid[3][3], int depth, int player_id, int maximizing_player, int *best_row, int *best_col) {
    int score = evaluate_board(grid, player_id);
    
    if (depth == 0 || grid_checker(grid) == 0) {
        return score;
    }
    
    if (maximizing_player) {
        int max_score = INT_MIN;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[i][j].couleur == 0) {
                    grid[i][j].couleur = player_id;
                    
                    int current_score = minimax(grid, depth - 1, player_id, 0, best_row, best_col);
                    
                    grid[i][j].couleur = 0;
                    
                    if (current_score > max_score) {
                        max_score = current_score;
                        *best_row = i;
                        *best_col = j;
                    }
                }
            }
        }
        return max_score;
    } else {
        int min_score = INT_MAX;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[i][j].couleur == 0) {
                    grid[i][j].couleur = 1;
                    
                    int current_score = minimax(grid, depth - 1, player_id, 1, best_row, best_col);
                    
                    grid[i][j].couleur = 0;
                    
                    if (current_score < min_score) {
                        min_score = current_score;
                        *best_row = i;
                        *best_col = j;
                    }
                }
            }
        }
        return min_score;
    }
}