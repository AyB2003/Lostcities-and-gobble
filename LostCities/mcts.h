#ifndef MCTS_H
#define MCTS_H


typedef struct {
    cards hand[8];
    cards expedition1[5][12];
    cards expedition2[5][12];
    int top[5];
    cards discard[5][12];
    int top_discard[5];
    int deck_size;
} GameState;

typedef struct MCTSNode {
    GameState state;
    struct MCTSNode* parent;
    struct MCTSNode** children;
    int num_children;
    int visits;
    double wins;
} MCTSNode;

typedef struct {
    int hand_index;
    int destination;
    bool draw_from_discard;
    int discard_pile_index;
} Action;
void displaygamestate(GameState* state);
MCTSNode* monte_carlo_tree_search(GameState root_state, int iterations); 

#endif