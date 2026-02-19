#pragma once
#include "Board.h"

#define CHECKMATE_SCORE  1000000
#define STALEMATE_SCORE  0
#define MAX_DEPTH        5

typedef struct {
	int start; // start square index (0-63)
	int end;   // end square index (0-63)
	int score; // evaluation score for this move (used in find_best_move)
} Move;

typedef struct {
	int start;
	int end;
	int score;
} ScoredMove;

int collect_moves(Board* b, ScoredMove* moves);
int score_move(Board* b, int start, int end);
int evaluate_board(Board* b);
Move find_best_move(Board* b, int depth);
int minimax(Board b, int depth, int alpha, int beta, int is_maximizing);