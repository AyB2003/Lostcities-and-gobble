#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "animation.h"
#include "mcts.h"
#include <time.h>



bool is_terminal(GameState state) {
    return state.deck_size == 0;
}
void displaygamestate(GameState* state){
    printf ("The hand is : ");
    for (int i = 0; i < 8 ; i++){
        printf("[n: %d, c :%d] ", state->hand[i].nombre, state->hand[i].couleur);
    }
    printf("\n");
    printf ("The expidition to each color is: \n");
    for (int j = 0; j < 12; j++){
        for (int i = 0; i < 5; i++){
            printf("[n: %d,c : %d]", state->expedition1[i][j].nombre, state->expedition1[i][j].couleur);
        }
        printf("\n");
    }
    printf("The piles are : \n");
    printf("Pile 1        Pile 2        Pile 3        Pile 4        Pile 5\n");
    for (int j = 0; j < 12; j++){
        for (int i = 0; i < 5; i++){
            printf("[n: %d,c : %d]", state->discard[i][j].nombre, state->discard[i][j].couleur);
        }
        printf("\n");
    }
    printf("\n");
}

cards draw_next_card_from_deck() {
    return (cards){ rand() % 10 + 1, rand() % 5, {0, 0, 100, 150}, NULL };
}


GameState generate_new_state(GameState state, Action action) {
    GameState new_state = state;

    cards card = new_state.hand[action.hand_index];
    bool valid_move = false;

    if (action.destination >= 0 && action.destination < 5) {
        int expedition_index = action.destination;
        if (new_state.top[expedition_index] == 0 || 
            (new_state.expedition1[expedition_index][new_state.top[expedition_index] - 1].nombre < card.nombre &&
            new_state.expedition1[expedition_index][new_state.top[expedition_index] - 1].couleur == card.couleur)) {
            new_state.expedition1[expedition_index][new_state.top[expedition_index]++] = card;
            valid_move = true;
        }
    } else if (action.destination >= 5 && action.destination < 10) {
        int discard_index = action.destination - 5;
        if (card.couleur == discard_index) {
            new_state.discard[discard_index][new_state.top_discard[discard_index]++] = card;
            valid_move = true;
        }
    }

    if (!valid_move) {
        return state;
    }
    for (int i = action.hand_index; i < 7; i++) {
        new_state.hand[i] = new_state.hand[i + 1];
    }
    new_state.hand[7] = (cards){0};

    if (action.draw_from_discard) {
        if (new_state.top_discard[action.discard_pile_index] > 0) {
            new_state.hand[7] = new_state.discard[action.discard_pile_index][--new_state.top_discard[action.discard_pile_index]];
        }
    } else {
        if (new_state.deck_size > 0) {
            new_state.hand[7] = draw_next_card_from_deck();
            new_state.deck_size--;
        }
    }

    return new_state;
}



GameState perform_random_action(GameState state) {
    Action possible_actions[8 * 10 * 2 * 5];
    int num_possible_actions = 0;
    
    for (int i = 0; i < 8; i++) {
        cards card = state.hand[i];
        for (int j = 0; j < 5; j++) { 
            if (state.top[j] == 0 || 
                (state.expedition1[j][state.top[j] - 1].nombre < card.nombre &&
                state.expedition1[j][state.top[j] - 1].couleur == card.couleur)) {
                for (int k = 0; k < 2; k++) { 
                    for (int l = 0; l < 5; l++) {
                        Action action = {i, j, k, l};
                        possible_actions[num_possible_actions++] = action;
                    }
                }
            }
        }
        for (int j = 5; j < 10; j++) {
            int discard_index = j - 5;
            if (card.couleur == discard_index) {
                for (int k = 0; k < 2; k++) {
                    for (int l = 0; l < 5; l++) {
                        Action action = {i, j, k, l};
                        possible_actions[num_possible_actions++] = action;
                    }
                }
            }
        }
    }

    if (num_possible_actions > 0) {
        Action action = possible_actions[rand() % num_possible_actions];
        return generate_new_state(state, action);
    }

    return state;
}





double evaluate(GameState state) {

    int score1 = 0, score2 = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < state.top[i]; j++) {
            score1 += state.expedition1[i][j].nombre;
        }
        for (int j = 0; j < state.top[i]; j++) {
            score2 += state.expedition2[i][j].nombre;
        }
    }
    return score1 - score2;
}

MCTSNode* create_node(GameState state, MCTSNode* parent) {
    MCTSNode* node = (MCTSNode*)malloc(sizeof(MCTSNode));
    node->state = state;
    node->parent = parent;
    node->children = NULL;
    node->num_children = 0;
    node->visits = 0;
    node->wins = 0.0;
    return node;
}

MCTSNode* select_best_child(MCTSNode* node) {
    double best_uct = -1.0;
    MCTSNode* best_child = NULL;
    
    for (int i = 0; i < node->num_children; i++) {
        MCTSNode* child = node->children[i];
        double uct = (child->wins / child->visits) + 
                     sqrt(2 * log(node->visits) / child->visits);
        if (uct > best_uct) {
            best_uct = uct;
            best_child = child;
        }
    }
    
    return best_child;
}

MCTSNode* selection(MCTSNode* root) {
    MCTSNode* node = root;
    while (node->num_children != 0) {
        node = select_best_child(node);
    }
    return node;
}

void expand(MCTSNode* node) {
    int num_possible_actions = 0;
    Action possible_actions[8 * 10 * 2 * 5];
    
    for (int i = 0; i < 8; i++) {
        cards card = node->state.hand[i];
        for (int j = 0; j < 5; j++) {
            if (node->state.top[j] == 0 || 
                (node->state.expedition1[j][node->state.top[j] - 1].nombre < card.nombre &&
                node->state.expedition1[j][node->state.top[j] - 1].couleur == card.couleur)) {
                for (int k = 0; k < 2; k++) {
                    for (int l = 0; l < 5; l++) {
                        Action action = {i, j, k, l};
                        possible_actions[num_possible_actions++] = action;
                    }
                }
            }
        }
        for (int j = 5; j < 10; j++) {
            int discard_index = j - 5;
            if (card.couleur == discard_index) {
                for (int k = 0; k < 2; k++) {
                    for (int l = 0; l < 5; l++) {
                        Action action = {i, j, k, l};
                        possible_actions[num_possible_actions++] = action;
                    }
                }
            }
        }
    }

    node->children = (MCTSNode**)malloc(sizeof(MCTSNode*) * num_possible_actions);
    node->num_children = num_possible_actions;
    for (int i = 0; i < num_possible_actions; i++) {
        GameState new_state = generate_new_state(node->state, possible_actions[i]);
        node->children[i] = create_node(new_state, node);
    }
}




double simulate(GameState state) {
    while (!is_terminal(state)) {
        state = perform_random_action(state);
    }
    return evaluate(state);
}

void backpropagation(MCTSNode* node, double result) {
    while (node != NULL) {
        node->visits += 1;
        node->wins += result;
        node = node->parent;
    }
}

MCTSNode* monte_carlo_tree_search(GameState root_state, int iterations) {
    MCTSNode* root = create_node(root_state, NULL);
    
    for (int i = 0; i < iterations; i++) {
        MCTSNode* node = selection(root);
        if (!is_terminal(node->state)) {
            expand(node);
        }
        MCTSNode* child = (node->num_children == 0) ? node : node->children[0];
        double result = simulate(child->state);
        backpropagation(child, result);
    }
    
    return select_best_child(root);
}






