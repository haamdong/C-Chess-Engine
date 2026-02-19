#include <stdio.h>
#include <string.h>
#include "Board.h"
#include "engine.h"

typedef enum { MODE_PVP, MODE_PVE, MODE_ANALYZE } GameMode;

void apply_move(Board* b, int start, int end) {
    int ep_target = b->en_passant;

    update_castling_rights(b, start, end);
    b->squares[end] = b->squares[start];
    b->squares[start] = EMPTY;

    int piece = b->squares[end];

    // En Passant capture
    if ((piece == W_PAWN || piece == B_PAWN) && end == ep_target) {
        int captured_sq = end + (b->turn == 0 ? -8 : 8);
        b->squares[captured_sq] = EMPTY;
    }

    // Pawn promotion
    if (piece == W_PAWN && end / 8 == 7 ||
        piece == B_PAWN && end / 8 == 0) {
        pawn_promotion(b, end);
    }

    b->turn ^= 1;

	// reset en passant target
    if (b->en_passant == ep_target)
        b->en_passant = -1;
}

int handle_input(Board* b) {
    char input[10] = "";
    int start, end;

    printf("Enter move (e.g. e2e4 / O-O / O-O-O): ");
    if (!scanf("%9s", input)) return 0;

    if (strcmp(input, "O-O") == 0 || strcmp(input, "O-O-O") == 0) {
        start = 4 + 56 * b->turn;
        end = (strcmp(input, "O-O") == 0) ? 6 : 2;
        end += 56 * b->turn;
        if (!move_castle(b, start, end)) {
            printf("Invalid castling.\n");
            return 0;
        }
        return 1;
    }

    algebraic_to_idx(input, &start, &end);
    if (!move_piece(b, start, end)) {
        printf("Invalid move.\n");
        return 0;
    }
    apply_move(b, start, end);
    return 1;
}

int check_game_over(Board* b) {
    if (is_checkmate(b)) {
        show_board(b);
        printf("%s wins by checkmate!\n", b->turn == 1 ? "White" : "Black");
        return 1;
    }
    if (is_stalemate(b)) {
        show_board(b);
        printf("Stalemate! Draw.\n");
        return 1;
    }
    return 0;
}

void run_pvp(Board* b) {
    while (1) {
        show_board(b);
        printf("%s's turn\n", b->turn == 0 ? "White" : "Black");
        handle_input(b);
        if (check_game_over(b)) break;
    }
}

void run_pve(Board* b, int player_color, int depth) {
    while (1) {
        show_board(b);
        if (b->turn == player_color) {
            printf("Your turn: ");
            handle_input(b);
        }
        else {
            printf("Engine thinking (depth %d)...\n", depth);
            Move best = find_best_move(b, depth);
            if (best.start == -1) break;
            printf("Engine: %c%d%c%d\n",
                'a' + best.start % 8, best.start / 8 + 1,
                'a' + best.end % 8, best.end / 8 + 1);
            apply_move(b, best.start, best.end);
			printf("Engine evaluation: %.3lf\n", (double)best.score/100.0);
        }
        if (check_game_over(b)) break;
    }
}

void run_analyze(Board* b, int depth) {
    while (1) {
        show_board(b);

        printf("[%s] Best move: ", b->turn == 0 ? "White" : "Black");
        Move best = find_best_move(b, depth);

        if (best.start == -1) { 
            printf("none\n");
            break; 
        }

        printf("%c%d%c%d (score: %d)\n",
            'a' + best.start % 8, best.start / 8 + 1,
            'a' + best.end % 8, best.end / 8 + 1,
            best.score);

        printf("Apply move? (y/n): ");

        char c; 
        scanf(" %c", &c);

        if (c == 'y') apply_move(b, best.start, best.end);
        if (check_game_over(b)) break;
    }
}

int main() {
    Board chess_board = {
        .castle = {1, 1, 1, 1},
        .en_passant = -1,
        .turn = 0
    };
    init_board(&chess_board);

    printf("=== Chess ===\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Engine\n");
    printf("3. Analysis Mode\n");
    printf("Select: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        run_pvp(&chess_board);
    }
    else if (choice == 2) {
        int color;
        printf("Play as (0=White, 1=Black): ");
        scanf("%d", &color);
        run_pve(&chess_board, color, MAX_DEPTH);
    }
    else {
        run_analyze(&chess_board, MAX_DEPTH);
    }

    return 0;
}