#include <stdio.h>
#include <stdlib.h>

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

enum {
   EMPTY, W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
   B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

typedef struct Board {
   int squares[64];
   int turn;       // White = 0, Black = 1
   int castle;     // Castling Right
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
   char piece_chars[] = ".PNBRQKpnbrqk";
   for (int rank = 7; rank >= 0; rank--) {
      printf("%d ", rank + 1);
      for (int file = 0; file < 8; file++) {
         printf("%c ", piece_chars[b->squares[rank * 8 + file]]);
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

static int locate_king(Board* b) {
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
}

static int is_checked_on_straight_line(Board* b, int king_pos) {
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

static int is_checked_on_diagonal(Board* b, int king_pos) {
   for (int d = 0; d < 4; d++) {
      int dir = diagonal_offsets[d];
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

static int is_checked_by_pawn(Board* b, int king_pos) {
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

static int is_checked_by_knight(Board* b, int king_pos) {
   int origin_file = king_pos % 8;
   int origin_rank = king_pos / 8;

   for (int i = 0; i < 8; i++) {
      int target_idx = king_pos + knight_offsets[i];
      int target_file = target_idx % 8;
      int target_rank = target_idx / 8;

      if (target_idx < 0 || target_idx > 63) continue;

      int piece = b->squares[target_idx];
      int piece_color = (piece >= B_PAWN) ? 1 : 0;

      // Prevent wrapped cases
      if (abs(origin_file - target_file) > 2 || abs(origin_rank - target_rank) > 2) continue;

      // Enemy piece: Check if it can attack
      int type = (piece > W_KING) ? piece - 6 : piece;

      if (piece_color != b->turn && type == W_KNIGHT) return 1;
   }

   return 0;
}

static int is_checked_by_king(Board* b, int king_pos) {
   // Stricktly speaking, king cannot be checked by opponent's king
   // This function is for checking illegal move (case that king moves to square where king can be captured by opponent.)
   for (int i = 0; i < 8; i++) {
      int target_idx = king_pos + king_offsets[i];

      if (target_idx > 63 || target_idx < 0) continue;
   }

   return 0;
}

int is_checked(Board* b) {
   // Check if the King is under attack: Diagonal (Bishop/Queen/Pawn), Straight (Rook/Queen), or L-shape (Knight)
   int king_pos = locate_king(b);
   
   if (is_checked_on_straight_line(b, king_pos) ||
      is_checked_on_diagonal(b, king_pos)       ||
      is_checked_by_pawn(b, king_pos)           ||
      is_checked_by_knight(b, king_pos)         ||
      is_checked_by_king(b,king_pos)) {
      return 1;
   }

   return 0;
}

static int is_valid_pawn_move(Board* b, int start, int end) {
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
      if (target == EMPTY) return 1;
   }

   // CASE 2: Move forward 2 squares (must be at starting rank, path clear)
   if (diff == 16 * direction) {
      int mid = start + (8 * direction); // Intermediate point
      if (rank == start_rank && b->squares[mid] == EMPTY && target == EMPTY) {
         return 1;
      }
   }

   // CASE 3: Diagonal capture (enemy piece must be present)
   // Diagonals are +-7, +-9 for both sides
   if (diff == 7 * direction || diff == 9 * direction) {
      int start_file = start % 8;
      int end_file = end % 8;

      if (abs(start_file - end_file) != 1) return 0;

      if (target != EMPTY) {
         return 1;
      }
      // TODO: Add En Passant logic
   }

   return 0;
}

static int is_valid_bishop_move(Board* b, int start, int end) {
   // Previously checked if (diff % 7 == 0 || diff % 9 == 0),
   // but changed to slope check to avoid wrapping issues across the board.
   int start_rank = start / 8;
   int start_file = start % 8;
   int end_rank = end / 8;
   int end_file = end % 8;
   int diff;

   // Return 0 if absolute slope is not 1
   if (abs(start_rank - end_rank) != abs(start_file - end_file)) return 0;

   // Calculate movement direction (+9, +7, -9, -7)
   if (end_rank > start_rank && end_file > start_file) diff = 9;
   if (end_rank > start_rank && end_file < start_file) diff = 7;
   if (end_rank < start_rank && end_file > start_file) diff = -9;
   if (end_rank < start_rank && end_file < start_file) diff = -7;

   for (int i = start + diff; i != end; i += diff) {
      if (b->squares[i] != EMPTY) return 0;
   }

   return 1;
}

static int is_valid_knight_move(Board* b, int start, int end) {
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

static int is_valid_rook_move(Board* b, int start, int end) {
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

static int is_valid_queen_move(Board* b, int start, int end) {
   // Queen can move like both Rook and Bishop.
   // If the move is valid for either piece then it is valid move for Queen.

   if (is_valid_bishop_move(b, start, end) || is_valid_rook_move(b, start, end)) {
      return 1;
   }

   return 0;
}

static int is_valid_king_move(Board* b, int start, int end) {

}

static int is_valid_piece_move(Board* b, int start, int end) {
   int piece = b->squares[start];

   // Check pawn validity 
   if (piece == B_PAWN || piece == W_PAWN) {
      return is_valid_pawn_move(b, start, end);
   }
   // Check bishop validity
   if (piece == B_BISHOP || piece == W_BISHOP) {
      return is_valid_bishop_move(b, start, end);
   }
   // Check knight validity
   if (piece == B_KNIGHT || piece == W_KNIGHT) {
      return is_valid_knight_move(b, start, end);
   }
   // Check rook validity
   if (piece == B_ROOK || piece == W_ROOK) {
      return is_valid_rook_move(b, start, end);
   }
   // Check queen validity
   if (piece == B_QUEEN || piece == W_QUEEN) {
      return is_valid_queen_move(b, start, end);
   }
   // Check king validity
   if (piece == B_KING || piece == W_KING) {
      return is_valid_king_move(b, start, end);
   }
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
   // 4. Attempted to move while in check (TODO)


   // 5. Attempted to move a pinned piece (TODO)

   // Check all piece's validity
   return is_valid_piece_move(b, start, end);
}

int is_checkmate(Board* b) {

}

int main() {
   Board chess_board;
   int start_idx, end_idx;
   char input[5];

   init_board(&chess_board); // Set initial state

   while (1) {
      show_board(&chess_board); // Display current board

      printf("\n Enter move (e.g., e2e4): ");
      scanf("%s", input);

      // Convert input string to index
      algebraic_to_idx(input, &start_idx, &end_idx);

      // Check if move is illegal
      if (move_piece(&chess_board, start_idx, end_idx) == 0) {
         printf("Invalid move.\n");
      }
      // Update board if move is legal
      else {
         chess_board.squares[end_idx] = chess_board.squares[start_idx];
         chess_board.squares[start_idx] = EMPTY;
         chess_board.turn = chess_board.turn == 0 ? 1 : 0;
      }

      if (is_checkmate(&chess_board)) {
         if (chess_board.turn == 0) {
            printf("White Wins!\n");
         }
         else {
            printf("Black Wins!\n");
         }
         break;
      }
   }

   
   return 0;
}