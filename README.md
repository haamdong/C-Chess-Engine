# C-Chess-Engine

C-Chess-Engine is a chess engine implemented in pure C.
This project is designed for educational purposes and demonstrates how a classical chess engine works internally, including board representation, move generation, evaluation, and search algorithms.

------------------------------------------------------------

Overview

The engine implements the core logic required to play standard chess on an 8x8 board.
It calculates legal moves, evaluates positions, and searches for the best move using the Minimax algorithm with Alpha-Beta pruning.

------------------------------------------------------------

Main Features

- Standard 8x8 chess board representation
- Legal move generation
- Material-based evaluation function
- Minimax search
- Alpha-Beta pruning
- Command-line interaction
- Modular C source structure

------------------------------------------------------------

Engine Architecture

Board Representation
Maintains the current game state, stores piece locations, and manages turn information.

Move Generation
Generates pseudo-legal moves and filters illegal moves that leave the king in check.
Handles castling, en passant, and promotion.

Search
Implements depth-limited Minimax with Alpha-Beta pruning to efficiently explore the game tree.

Evaluation
Assigns a numerical score to a board position based on material and optional positional heuristics.

------------------------------------------------------------

Build Instructions

Requirements
- GCC or Clang compiler

Example compilation command

gcc -o chess main.c board.c move.c search.c evaluate.c

Adjust file names if your structure differs.

Run

./chess

------------------------------------------------------------

Usage

1. Run the executable.
2. Enter moves in coordinate notation (example: e2e4).
3. The engine calculates and outputs its best move.
4. The game continues until checkmate, stalemate, or draw.

------------------------------------------------------------

Example Project Structure

C-Chess-Engine
│
├── main.c                Program entry point
├── board.c / board.h     Board state management
├── move.c / move.h       Move generation logic
├── search.c / search.h   Search algorithm
├── evaluate.c / evaluate.h  Position evaluation
└── README.md

------------------------------------------------------------

Future Improvements

- UCI protocol support
- Transposition tables with Zobrist hashing
- Iterative deepening
- Improved move ordering
- Quiescence search
- Opening book
- Endgame tablebases
- Performance optimization

------------------------------------------------------------

License
