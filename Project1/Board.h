#pragma once

// Array of direction offsets (extern declaration for shared use)
extern int orthogonal_offsets[];
extern int diagonal_offsets[];
extern int pawn_offsets[];
extern int knight_offsets[];
extern int king_offsets[];

// Enums for pieces and castling rights
enum pieces {
    EMPTY, W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

enum castle {
    W_KINGSIDE, W_QUEENSIDE, B_KINGSIDE, B_QUEENSIDE
};

// Board structure definition
typedef struct Board {
    int squares[64];
    int turn;       // White = 0, Black = 1
    int castle[4];  // Castling rights
    int en_passant; // En Passant target square
} Board;

// Core Game Functions
void init_board(Board* b);
void show_board(Board* b);
void algebraic_to_idx(char* input, int* start, int* end);
int move_piece(Board* b, int start, int end);
int move_castle(Board* b, int start, int end);
void pawn_promotion(Board* b, int target_idx);
int is_checkmate(Board* b);
int is_stalemate(Board* b);
void update_castling_rights(Board* b, int start_idx, int end_idx);

// Validation & Helper Functions
int locate_king(Board* b);
int is_checked_on_straight_line(Board* b, int king_pos);
int is_checked_on_diagonal(Board* b, int king_pos);
int is_checked_by_pawn(Board* b, int king_pos);
int is_checked_by_knight(Board* b, int king_pos);
int is_checked_by_king(Board* b, int king_pos);
int is_checked(Board* b);
int is_king_safe_after_move(Board* b, int start, int end);
int is_valid_pawn_move(Board* b, int start, int end);
int is_valid_bishop_move(Board* b, int start, int end);
int is_valid_knight_move(Board* b, int start, int end);
int is_valid_rook_move(Board* b, int start, int end);
int is_valid_queen_move(Board* b, int start, int end);
int is_valid_king_move(Board* b, int start, int end);
int is_valid_piece_move(Board* b, int start, int end);