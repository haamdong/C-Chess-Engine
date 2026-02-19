#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "Engine.h"

int piece_values[] = { 0, 100, 320, 334, 500, 900, 20000, -100, -320, -334, -500, -900, -20000 };

// Pawn position bonus table (White perspective, index 0 = a1)
int pawn_table[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10,-20,-20, 10, 10,  5,
     5, -5,-10,  0,  0,-10, -5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
     0,  0,  0,  0,  0,  0,  0,  0
};

// Knight position bonus table (White perspective, index 0 = a1)
int knight_table[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

// Bishop position bonus table (White perspective, index 0 = a1)
int bishop_table[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10, 5 ,  0,  0,  0,  0,  0, -10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

// Rook position bonus table (White perspective, index 0 = a1)
int rook_table[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  5,  5,  0,  0,  0
};

// Queen position bonus table (White perspective, index 0 = a1)
int queen_table[64] = {
    -20,-10,-10, -5, -5,-10, -10, -20,
    -10,  0,  0,  0,  0,  0,   0, -10,
    -10,  0,  5,  5,  5,  5,   0, -10,
     -5,  0,  5,  5,  5,  5,   0,  -5,
      0,  0,  5,  5,  5,  5,   0,  -5,
    -10,  5,  5,  5,  5,  5,   0, -10,
    -10,  0, -5, -5, -5, -5, -10, -10,
    -20,-10,-10,-10,-10,-10, -10, -20
};

int king_safety(Board* b, int king_sq, int color) {
    int score = 0;

    // Bonus if castling rights remain (king still has safe options)
    if (color == 0) {
        if (b->castle[W_KINGSIDE] || b->castle[W_QUEENSIDE]) score += 10;
    }
    else {
        if (b->castle[B_KINGSIDE] || b->castle[B_QUEENSIDE]) score += 10;
    }

    // Bonus for pawn shield in front of king
    int pawn = (color == 0) ? W_PAWN : B_PAWN;
    int dir = (color == 0) ? 8 : -8;
    int king_file = king_sq % 8;

    for (int df = -1; df <= 1; df++) {
        int file = king_file + df;
        if (file < 0 || file > 7) continue;
        int shield_sq = king_sq + dir + df;
        if (shield_sq >= 0 && shield_sq < 64 && b->squares[shield_sq] == pawn)
            score += 15; // Pawn shield bonus
    }

    return score;
}

int evaluate_board(Board* b) {
    int score = 0;
    for (int i = 0; i < 64; i++) {
        int piece = b->squares[i];
        if (piece == EMPTY) continue;

        score += piece_values[piece];

        if (piece == W_PAWN)   score += pawn_table[i];
        else if (piece == B_PAWN)   score -= pawn_table[63 - i];
        else if (piece == W_KNIGHT) score += knight_table[i];
        else if (piece == B_KNIGHT) score -= knight_table[63 - i];
        else if (piece == W_BISHOP) score += bishop_table[i];
        else if (piece == B_BISHOP) score -= bishop_table[63 - i];
        else if (piece == W_ROOK)   score += rook_table[i];
        else if (piece == B_ROOK)   score -= rook_table[63 - i];
        else if (piece == W_QUEEN)  score += queen_table[i];
        else if (piece == B_QUEEN)  score -= queen_table[63 - i];
    }

    // King safety evaluation
    int w_king = -1, b_king = -1;
    for (int i = 0; i < 64; i++) {
        if (b->squares[i] == W_KING) w_king = i;
        if (b->squares[i] == B_KING) b_king = i;
    }
    if (w_king != -1) score += king_safety(b, w_king, 0);
    if (b_king != -1) score -= king_safety(b, b_king, 1);

    return score;
}

// Score a move for ordering: higher score = search first
int score_move(Board* b, int start, int end) {
    int target = b->squares[end];
    if (target != EMPTY) {
        // Prioritize captures: value of captured piece - value of attacker
        int attacker = b->squares[start];
        return piece_values[target] - piece_values[attacker] + 10000;
    }
    return 0;
}

// Collect and sort all legal moves for the current position
int collect_moves(Board* b, ScoredMove* moves) {
    int count = 0;

    for (int start = 0; start < 64; start++) {
        for (int end = 0; end < 64; end++) {
            if (move_piece(b, start, end)) {
                moves[count++] = (ScoredMove){ start, end, score_move(b, start, end) };
            }
        }
    }

    // Add castling moves
    int king_sq = 4 + 56 * b->turn;
    int kingside = king_sq + 2;
    int queenside = king_sq - 2;

    Board tmp = *b;
    if (move_castle(&tmp, king_sq, kingside))
        moves[count++] = (ScoredMove){ king_sq, kingside, 50 };

    tmp = *b;
    if (move_castle(&tmp, king_sq, queenside))
        moves[count++] = (ScoredMove){ king_sq, queenside, 50 };

    // Sort moves by score descending (best first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (moves[j].score > moves[i].score) {
                ScoredMove tmp2 = moves[i];
                moves[i] = moves[j];
                moves[j] = tmp2;
            }
        }
    }

    return count;
}

/// <summary>
/// Minimax algorithm with alpha-beta pruning.
/// Board is passed by value so no undo logic is needed.
/// </summary>
/// <param name="b">Current board state (copy)</param>
/// <param name="depth">Remaining search depth</param>
/// <param name="alpha">Best score the maximizer can guarantee</param>
/// <param name="beta">Best score the minimizer can guarantee</param>
/// <param name="is_maximizing">1 to maximize, 0 to minimize</param>
/// <returns>Evaluation score of the position</returns>
int minimax(Board b, int depth, int alpha, int beta, int is_maximizing) {
    if (is_checkmate(&b)) return is_maximizing ? -CHECKMATE_SCORE : CHECKMATE_SCORE;
    if (is_stalemate(&b)) return STALEMATE_SCORE;
    if (depth == 0)       return evaluate_board(&b);

    ScoredMove moves[256];
    int count = collect_moves(&b, moves);

    if (is_maximizing) {
        int max_eval = -CHECKMATE_SCORE - 1;
        for (int i = 0; i < count; i++) {
            Board next = b;
            next.squares[moves[i].end] = next.squares[moves[i].start];
            next.squares[moves[i].start] = EMPTY;
            update_castling_rights(&next, moves[i].start, moves[i].end);
            next.turn ^= 1;

            int eval = minimax(next, depth - 1, alpha, beta, 0);
            if (eval > max_eval) max_eval = eval;
            if (eval > alpha)    alpha = eval;
            if (beta <= alpha)   break; // Beta cutoff
        }
        return max_eval;
    }
    else {
        int min_eval = CHECKMATE_SCORE + 1;
        for (int i = 0; i < count; i++) {
            Board next = b;
            next.squares[moves[i].end] = next.squares[moves[i].start];
            next.squares[moves[i].start] = EMPTY;
            update_castling_rights(&next, moves[i].start, moves[i].end);
            next.turn ^= 1;

            int eval = minimax(next, depth - 1, alpha, beta, 1);
            if (eval < min_eval) min_eval = eval;
            if (eval < beta)     beta = eval;
            if (beta <= alpha)   break; // Alpha cutoff
        }
        return min_eval;
    }
}

Move find_best_move(Board* b, int depth) {
    int is_maximizing = (b->turn == 0); // White maximizes, Black minimizes
    Move best = { -1, -1, is_maximizing ? -2000000 : 2000000 };

    for (int start = 0; start < 64; start++) {
        for (int end = 0; end < 64; end++) {
            if (!move_piece(b, start, end)) continue;

            Board next = *b;
            next.squares[end] = next.squares[start];
            next.squares[start] = EMPTY;
            update_castling_rights(&next, start, end);
            next.turn ^= 1;

            int eval = minimax(next, depth - 1,
                -2000000, 2000000, !is_maximizing);

            if (is_maximizing ? eval > best.score : eval < best.score) {
                best.start = start;
                best.end = end;
                best.score = eval;
            }
        }
    }
    return best;
}