#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Chess Board Index Map (1D Array)

  [File]  a   b   c   d   e   f   g   h
  [Rank]
    8    56, 57, 58, 59, 60, 61, 62, 63
    7    48, 49, 50, 51, 52, 53, 54, 55
    6    40, 41, 42, 43, 44, 45, 46, 47
    5    32, 33, 34, 35, 36, 37, 38, 39
    4    24, 25, 26, 27, 28, 29, 30, 31
    3    16, 17, 18, 19, 20, 21, 22, 23
    2     8,  9, 10, 11, 12, 13, 14, 15
    1     0,  1,  2,  3,  4,  5,  6,  7
*/

// Array of direction offsets: Up, Down, Right, Left
int orthogonal_offsets[] = { 8, -8, 1, -1 };

// Array of diagonal offsets: Up-Left (-9), Up-Right (-7), Down-Left (+7), Down-Right (+9)
int diagonal_offsets[] = { -9, -7, 7, 9 };

// Array of pawn movements offsets
int pawn_offsets[] = { 7, 9 };

// Array of knight movements offsets
int knight_offsets[] = { 15, 17, -15, -17, 6, 10, -6, -10 };

// Array of king movements offsets
int king_offsets[] = { 1, -1, 7, -7, 8, -8, 9, -9 };

enum pieces {
    EMPTY, W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

enum castle {
    W_KINGSIDE, W_QUEENSIDE, B_KINGSIDE, B_QUEENSIDE
};

typedef struct Board {
    int squares[64];
    int turn;       // White = 0, Black = 1
    int castle[4];   // Castling Right
    int en_passant; // En Passant target square
} Board;

void init_board(Board* b) {
    int* ptr = b->squares;

    for (int i = 16; i < 48; i++)
        b->squares[i] = EMPTY;

    // Place Pawns
    for (int i = 8; i < 16; i++)
        b->squares[i] = W_PAWN;
    for (int i = 48; i < 56; i++)
        b->squares[i] = B_PAWN;

    // Place Pieces
    b->squares[0] = W_ROOK;   b->squares[7] = W_ROOK; b->squares[56] = B_ROOK;  b->squares[63] = B_ROOK;
    b->squares[1] = W_KNIGHT; b->squares[6] = W_KNIGHT; b->squares[57] = B_KNIGHT; b->squares[62] = B_KNIGHT;
    b->squares[2] = W_BISHOP; b->squares[5] = W_BISHOP; b->squares[58] = B_BISHOP; b->squares[61] = B_BISHOP;
    b->squares[3] = W_QUEEN; b->squares[59] = B_QUEEN;
    b->squares[4] = W_KING; b->squares[60] = B_KING;

    b->turn = 0; // White moves first
}

void show_board(Board* b) {
    const char* piece_chars[] = {
        ".",   // 0: EMPTY
        "♟", "♞", "♝", "♜", "♛", "♚", // 1~6: White pieces
        "♙", "♘", "♗", "♖", "♕", "♔"  // 7~12: Black pieces
    };

    printf("\n");
    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++) {
            printf("%s ", piece_chars[b->squares[rank * 8 + file]]);
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

void algebraic_to_idx(char* input, int* start, int* end) {
    /* ex) input[0] is 'e', input[1] is '2', input[2] is 'e', input[3] is '4'
    'e' -> file(4), '2' -> rank(1)
     idx = rank * 8 + file
    */

    *start = (input[1] - '1') * 8 + (input[0] - 'a');
    *end = (input[3] - '1') * 8 + (input[2] - 'a');
}

int locate_king(Board* b) {
    if (b->turn == 0) {
        // finds where white king is
        for (int i = 0; i < 64; i++) {
            if (b->squares[i] == W_KING) return i;
        }
    }
    if (b->turn == 1) {
        // finds where black king is
        for (int i = 0; i < 64; i++) {
            if (b->squares[i] == B_KING) return i;
        }
    }
	return -1; // This should never happen in a valid game state, but it's good to have a fallback.
}

int is_checked_on_straight_line(Board* b, int king_pos) {
    for (int d = 0; d < 4; d++) {
        int dir = orthogonal_offsets[d];
        int current = king_pos;

        while (1) {
            current += dir;

            // 1. Boundary Check (Prevent wrapping around board)
            // If logic relies solely on 0-63 index, wrapping issues occur (ex, H file to A file).
            // A robust coordinate check is needed here.
            if (current < 0 || current > 63) break;

            // Specific check for horizontal moves to prevent wrapping
            if (dir == 1 && current % 8 == 0) break; // Wrapped to A file
            if (dir == -1 && current % 8 == 7) break; // Wrapped to H file

            int piece = b->squares[current];

            // 2. Empty square: Continue searching further
            if (piece == EMPTY) continue;

            // 3. Identify piece color (0: White, 1: Black)
            // Since B_PAWN starts at 7, anything >= B_PAWN is Black.
            int piece_color = (piece >= B_PAWN) ? 1 : 0;

            // 4. Checks it's friendly fire
            if (piece_color == b->turn) break;

            // 5. Enemy piece: Check if it can attack
            int type = (piece > W_KING) ? piece - 6 : piece;

            // 6. Check if the enemy is a Rook or Queen
            if (type == W_ROOK || type == W_QUEEN) {
                return 1; // Optimization: Found a threat, no need to look further in this direction
            }

            // If it's not a Rook or Queen (e.g., Bishop/Knight/Pawn), it blocks the view but doesn't check.
            break;
        }
    }
    return 0;
}

int is_checked_on_diagonal(Board* b, int king_pos) {
    for (int d = 0; d < 4; d++) {
        int dir = diagonal_offsets[d];
        int current = king_pos;

        while (1) {
            current += dir;

            // 1. Boundary Check (Prevent wrapping around board)
            // If logic relies solely on 0-63 index, wrapping issues occur (ex, H file to A file).
            // A robust coordinate check is needed here.
            if (current < 0 || current > 63) break;

            // Specific check for diagonal moves to prevent wrapping
            int prev_file = (current - dir) % 8;
            int curr_file = current % 8;
            if (abs(prev_file - curr_file) != 1) break;

            int piece = b->squares[current];

            // 2. Empty square: Continue searching further
            if (piece == EMPTY) continue;

            // 3. Identify piece color (0: White, 1: Black)
            // Since B_PAWN starts at 7, anything >= B_PAWN is Black.
            int piece_color = (piece >= B_PAWN) ? 1 : 0;

            // 4. Checks it's friendly fire
            if (piece_color == b->turn) break;

            // 5. Enemy piece: Check if it can attack
            int type = (piece > W_KING) ? piece - 6 : piece;

            // 6. Check if the enemy is a Bishop or Queen
            if (type == W_BISHOP || type == W_QUEEN) {
                return 1; // Optimization: Found a threat, no need to look further in this direction
            }

            // If it's not a Rook or Queen (e.g., Bishop/Knight/Pawn), it blocks the view but doesn't check.
            break;
        }
    }
    return 0;
}

int is_checked_by_pawn(Board* b, int king_pos) {
    int curr_file = king_pos % 8;
    int dir = (b->turn == 0) ? 1 : -1;

    for (int i = 0; i < 2; i++) {
        int target_idx = king_pos + pawn_offsets[i] * dir;

        if (target_idx > 63 || target_idx < 0) continue;

        int piece = b->squares[target_idx];
        int file = target_idx % 8;

        // Case 1: skip if it's wrapped
        if (abs(curr_file - file) > 1) continue;

        // Case 2: check if it's black pawn if its white's turn
        if (dir == 1) {
            if (piece == B_PAWN) return 1;
        }

        // Case 3: check if it's white pawn if its black's turn
        if (dir == -1) {
            if (piece == W_PAWN) return 1;
        }
    }
    return 0;
}

int is_checked_by_knight(Board* b, int king_pos) {
    int origin_file = king_pos % 8;

    for (int i = 0; i < 8; i++) {
        int target_idx = king_pos + knight_offsets[i];
        int target_file = target_idx % 8;

        if (target_idx < 0 || target_idx > 63) continue;

        int piece = b->squares[target_idx];
        int piece_color = (piece >= B_PAWN) ? 1 : 0;

        // Prevent wrapped cases
        if (abs(origin_file - target_file) > 2) continue;

        // Enemy piece: Check if it can attack
        int type = (piece > W_KING) ? piece - 6 : piece;

        if (piece_color != b->turn && type == W_KNIGHT) return 1;
    }

    return 0;
}

int is_checked_by_king(Board* b, int king_pos) {
    // Stricktly speaking, king cannot be checked by opponent's king
    // This function is for checking illegal move (case that king moves to square where king can be captured by opponent.)
    int origin_file = king_pos % 8;

    for (int i = 0; i < 8; i++) {
        int target_idx = king_pos + king_offsets[i];

        if (target_idx > 63 || target_idx < 0) continue;

        int target_file = target_idx % 8;
        if (abs(origin_file - target_file) > 1) continue;

        int piece = b->squares[target_idx];
        int target = (piece > W_KING) ? piece - 6 : piece;

        if (target == W_KING) return 1;
    }

    return 0;
}

int is_checked(Board* b) {
    // Check if the King is under attack: Diagonal (Bishop/Queen/Pawn), Straight (Rook/Queen), or L-shape (Knight)
    int king_pos = locate_king(b);

    if (is_checked_on_straight_line(b, king_pos) ||
        is_checked_on_diagonal(b, king_pos) ||
        is_checked_by_pawn(b, king_pos) ||
        is_checked_by_knight(b, king_pos) ||
        is_checked_by_king(b, king_pos)) {
        return 1;
    }

    return 0;
}

// pseudo-code for checking king safety
int is_king_safe_after_move(Board* b, int start, int end) {
    int moving_piece = b->squares[start];
    int piece_target = b->squares[end];

    // 1. Hypothetically make the move
    b->squares[end] = moving_piece;
    b->squares[start] = EMPTY;

    // 2. Check king safety
    int is_safe = !is_checked(b);

    // 3. Undo the move
    b->squares[start] = moving_piece;
    b->squares[end] = piece_target;

    return is_safe;
}

int move_castle(Board* b, int start, int end) {
    // King cannot be in check at the start of castling
    if (is_checked(b)) return 0;

    // Kingside castling
    if (end > start) {
        // Check castling rights
        if (b->castle[W_KINGSIDE + b->turn * 2] == 0) return 0;

        // Check path between king and rook 
        for (int i = start + 1; i < end; i++) {
            // Path must be clear
            if (b->squares[i] != EMPTY) return 0;
            // Squares the king passes through must not be under attack
            if (is_king_safe_after_move(b, i, i + 1) == 0) return 0;
        }
        // King moves 2 squares
        b->squares[end] = b->squares[start];
        b->squares[start] = EMPTY;

        // Rook moves 
        b->squares[end - 1] = b->squares[end + 1];
        b->squares[end + 1] = EMPTY;
    }
    // Queenside castling
    else {
        // Check castling rights
        if (b->castle[W_QUEENSIDE + b->turn * 2] == 0) return 0;

        int rook_pos = end - 2; // a1(0) or a8(56)
        for (int i = start - 1; i > rook_pos; i--) {
            // Path must be clear
            if (b->squares[i] != EMPTY) return 0;
        }

        for (int i = start - 1; i > end; i--) {
            // Squares the king passes through must not be under attack
            if (is_king_safe_after_move(b, i, i - 1) == 0) return 0;
        }
        b->squares[end] = b->squares[start];
        b->squares[start] = EMPTY;
        b->squares[end + 1] = b->squares[end - 2];
        b->squares[end - 2] = EMPTY;
    }
	// Change turn and update castling rights
	b->castle[W_KINGSIDE + b->turn * 2] = 0;
	b->castle[W_QUEENSIDE + b->turn * 2] = 0;
	b->turn = b->turn == 0 ? 1 : 0;

    return 1;
}

int is_valid_pawn_move(Board* b, int start, int end) {
    int piece = b->squares[start];
    int target = b->squares[end];

    // 1. Determine direction (White: 1, Black: -1)
    int direction = (b->turn == 0) ? 1 : -1;

    // 2. Calculate rank of current piece (0~7)
    int rank = start / 8;
    int start_rank = (b->turn == 0) ? 1 : 6; // White starts at rank 1, Black at rank 6

    int diff = end - start; // Index difference

    // CASE 1: Move forward 1 square (must be empty)
    if (diff == 8 * direction) {
        if (target == EMPTY) {
            return 1;
        }
    }

    // CASE 2: Move forward 2 squares (must be at starting rank, path clear)
    if (diff == 16 * direction) {
        int mid = start + (8 * direction); // Intermediate point
        if (rank == start_rank && b->squares[mid] == EMPTY && target == EMPTY) return 1;
    }

    // CASE 3: Diagonal capture (enemy piece must be present except en passant)
    // Diagonals are +-7, +-9 for both sides
    if (diff == 7 * direction || diff == 9 * direction) {
        int start_file = start % 8;
        int end_file = end % 8;

        if (abs(start_file - end_file) != 1) return 0;

        if (target == EMPTY && end == b->en_passant) {
            return 1;
        }

        if (target != EMPTY) {
            return 1;
        }
    }

    return 0;
}

int is_valid_bishop_move(Board* b, int start, int end) {
    int start_rank = start / 8;
    int start_file = start % 8;
    int end_rank = end / 8;
    int end_file = end % 8;
    int diff = 0;

    // Return 0 if absolute slope is not 1
    if (abs(start_rank - end_rank) != abs(start_file - end_file)) return 0;

    // Calculate movement direction (+9, +7, -9, -7)
    if (end_rank > start_rank && end_file > start_file) diff = 9;
    if (end_rank > start_rank && end_file < start_file) diff = 7;
    if (end_rank < start_rank && end_file < start_file) diff = -9;
    if (end_rank < start_rank && end_file > start_file) diff = -7;

    for (int i = start + diff; i != end; i += diff) {
        if (b->squares[i] != EMPTY) return 0;
    }

    return 1;
}

int is_valid_knight_move(Board* b, int start, int end) {
    // Knight moves in an 'L' shape: 
    // 2 squares in one direction and 1 square in the perpendicular direction.
    int start_file = start % 8, end_file = end % 8;
    int start_rank = start / 8, end_rank = end / 8;

    // Case 1: Move 2 files horizontally and 1 rank vertically
    if (abs(start_file - end_file) == 2) {
        if (abs(start_rank - end_rank) == 1) return 1;
    }

    // Case 2: Move 1 file horizontally and 2 ranks vertically
    if (abs(start_file - end_file) == 1) {
        if (abs(start_rank - end_rank) == 2) return 1;
    }

    return 0;
}

int is_valid_rook_move(Board* b, int start, int end) {
    int start_rank = start / 8;
    int start_file = start % 8;
    int end_rank = end / 8;
    int end_file = end % 8;

    // 1. Return 0 if not a linear move (neither same rank nor same file)
    if (start_rank != end_rank && start_file != end_file) return 0;
    // 2. Calculate step direction
    int step;

    if (start_rank == end_rank) {
        // Horizontal move: Index difference is 1 (Right: +1, Left: -1)
        step = (end > start) ? 1 : -1;
    }
    else {
        // Vertical move: Index difference is 8 (Up: +8, Down: -8)
        step = (end > start) ? 8 : -8;
    }

    // 3. Check for obstacles in the path
    // Loop from the next square to the square before the target
    for (int i = start + step; i != end; i += step) {
        if (b->squares[i] != EMPTY) return 0; // Path is blocked
    }

    return 1;
}

int is_valid_queen_move(Board* b, int start, int end) {
    // Queen can move like both Rook and Bishop.
    // If the move is valid for either piece then it is valid move for Queen.

    if (is_valid_bishop_move(b, start, end) || is_valid_rook_move(b, start, end)) {
        return 1;
    }

    return 0;
}

int is_valid_king_move(Board* b, int start, int end) {
    int diff = end - start;
    int start_file = start % 8;
    int end_file = end % 8;

    // 1. Prevent wrapping (e.g., jumping from file 'h' to 'a')
    if (abs(start_file - end_file) > 1) return 0;

    // 2. Check if the movement matches any of the king's valid offsets
    for (int i = 0; i < 8; i++) {
        if (diff == king_offsets[i]) return 1;
    }

    return 0;
}

int is_valid_piece_move(Board* b, int start, int end) {
    int piece = b->squares[start];
    int valid = 0;

    // check each piece's movement validity
    if (piece == B_PAWN || piece == W_PAWN)          valid = is_valid_pawn_move(b, start, end);
    else if (piece == B_BISHOP || piece == W_BISHOP) valid = is_valid_bishop_move(b, start, end);
    else if (piece == B_KNIGHT || piece == W_KNIGHT) valid = is_valid_knight_move(b, start, end);
    else if (piece == B_ROOK || piece == W_ROOK)     valid = is_valid_rook_move(b, start, end);
    else if (piece == B_QUEEN || piece == W_QUEEN)   valid = is_valid_queen_move(b, start, end);
    else if (piece == B_KING || piece == W_KING)     valid = is_valid_king_move(b, start, end);

    // if move is invalid for the piece, return 0
    if (!valid) return 0;

    // This code solves all issues related to king safety after the move (pinned piece, moving into check, etc.) im genius.
    if (is_king_safe_after_move(b, start, end) == 0) {
        return 0;
    }

    return 1;
}

int move_piece(Board* b, int start, int end) {
    int piece = b->squares[start];
    int target = b->squares[end];

    // 1. Selected an empty square
    if (piece == EMPTY) return 0;

    // 2. Attempted to move opponent's piece
    if (b->turn == 0 && piece >= B_PAWN) return 0;
    if (b->turn == 1 && piece <= W_KING) return 0;

    // 3. Friendly fire check (target square has own piece)
    if (target != EMPTY) {
        if (b->turn == 0 && target <= W_KING) return 0;
        if (b->turn == 1 && target >= B_PAWN) return 0;
    }

    // Check all piece's validity
    return is_valid_piece_move(b, start, end);
}

void pawn_promotion(Board* b, int target_idx) {
    int promotion_input = 0;
    int promotion_array[] = { W_QUEEN, W_ROOK, W_KNIGHT, W_BISHOP };

    printf("Pawn Promotion! Enter choice (1: Queen, 2: Rook, 3: Knight, 4: Bishop): ");

    while (1) {
        // Get user input and check if it's a valid integer
        if (scanf("%d", &promotion_input) != 1) {
            // Clear input buffer to prevent infinite loop
            while (getchar() != '\n');
            continue;
        }
        // Adjust for 0-based array indexing
        promotion_input--;

        // Validate the choice range (0-3)
        if (promotion_input >= 0 && promotion_input <= 3)
            break;

        printf("Invalid input! Try again.\n");
    }

    // Promote piece
    int base_piece = promotion_array[promotion_input];
    b->squares[target_idx] = base_piece + (6 * b->turn);
}

int is_checkmate(Board* b) {
    // 1. King is not in check, Then it's not checkmate.
    if (is_checked(b) == 0) return 0;

    // Determine the range of friendly pieces based on turn
    int my_piece_min = (b->turn == 0) ? W_PAWN : B_PAWN;
    int my_piece_max = (b->turn == 0) ? W_KING : B_KING;

    // 2. Brute force: Try moving every friendly piece to every square
    for (int start_idx = 0; start_idx < 64; start_idx++) {
        int piece = b->squares[start_idx];

        // If it's my piece
        if (piece >= my_piece_min && piece <= my_piece_max) {

            for (int target_idx = 0; target_idx < 64; target_idx++) {
                // If it returns 1, it's not checkmate.
                if (move_piece(b, start_idx, target_idx) == 1) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

int is_stalemate(Board* b) {
    // 1. If king is in check, Then it's not stalemate.
    if (is_checked(b) == 1) return 0;

    // Determine the range of friendly pieces based on turn
    int my_piece_min = (b->turn == 0) ? W_PAWN : B_PAWN;
    int my_piece_max = (b->turn == 0) ? W_KING : B_KING;

    // 2. Brute force: Try moving every friendly piece to every square
    for (int start_idx = 0; start_idx < 64; start_idx++) {
        int piece = b->squares[start_idx];

        // If it's my piece
        if (piece >= my_piece_min && piece <= my_piece_max) {

            for (int target_idx = 0; target_idx < 64; target_idx++) {
                // If it returns 1, it's not stalemate.
                if (move_piece(b, start_idx, target_idx) == 1) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void update_castling_rights(Board* b, int start_idx, int end_idx) {
    int piece = b->squares[start_idx];

    // If king (or at least any piece that's from king's starting square) moves, lose both castling rights
    if (start_idx == 4 || start_idx == 60) {
        b->castle[W_KINGSIDE + b->turn * 2] = 0;
        b->castle[W_QUEENSIDE + b->turn * 2] = 0;
    }

    // Update castling rights if rook moves from its starting position
    if (piece == W_ROOK && start_idx == 0)  b->castle[W_QUEENSIDE] = 0;
    if (piece == W_ROOK && start_idx == 7)  b->castle[W_KINGSIDE] = 0;
    if (piece == B_ROOK && start_idx == 56) b->castle[B_QUEENSIDE] = 0;
    if (piece == B_ROOK && start_idx == 63) b->castle[B_KINGSIDE] = 0;

    /* Update castling rights if rook is captured from its starting position
    * Strictly speaking, this doesn't check if rook is "captured" but just checks if piece is moved to the square where rook is originally placed.
    * This is because if rook is really captured, this code will function as intended.
    * And if rook is not captured but piece is moved to the square where rook is originally placed, then it's also fine to lose castling right
    * because even in that case, castling right should already been lost due to the fact that rook is moved from its original position.
    */
    if (end_idx == 0)  b->castle[W_QUEENSIDE] = 0;
    if (end_idx == 7)   b->castle[W_KINGSIDE] = 0;
    if (end_idx == 56) b->castle[B_QUEENSIDE] = 0;
    if (end_idx == 63)  b->castle[B_KINGSIDE] = 0;
}
