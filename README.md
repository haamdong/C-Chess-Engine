C-Chess-Engine

C-Chess-Engine is a chess engine implemented in pure C.
This project is designed for educational purposes and demonstrates how a classical chess engine works internally, including board representation, move generation, evaluation, and search algorithms.

Overview

The engine implements the core logic required to play standard chess on an 8x8 board. It calculates moves, evaluates positions, and searches for the best move using a tree-based algorithm.

Main Features

Standard 8x8 chess board representation
Legal move generation
Basic evaluation function based on material
Minimax search algorithm
Alpha-Beta pruning optimization
Command-line interaction
Modular C source structure

Engine Architecture

The engine is structured into several logical components.

Board Representation
Responsible for maintaining the current game state, storing piece locations, and managing turns.

Move Generation
Generates all possible pseudo-legal moves and filters out illegal moves that leave the king in check. Handles special rules such as castling, en passant, and promotion.

Search
Implements the Minimax algorithm with Alpha-Beta pruning to efficiently explore the game tree to a fixed depth.

Evaluation
Scores a board position using material values and optional positional heuristics.

Build Instructions

Requirements
GCC or Clang compiler

Example compilation command

gcc -o chess main.c board.c move.c search.c evaluate.c

Adjust the file names if your structure differs.

Run

./chess

Usage

Start the program from the terminal.
Enter moves using coordinate notation such as e2e4, depending on your implementation.
The engine will calculate and return its best move.
The game continues until checkmate, stalemate, or draw.

Example Project Structure

C-Chess-Engine
main.c – program entry point
board.c / board.h – board state management
move.c / move.h – move generation logic
search.c / search.h – search algorithm
evaluate.c / evaluate.h – position evaluation
README.md

Future Improvements

Support for UCI protocol
Transposition tables with Zobrist hashing
Iterative deepening
Improved move ordering
Quiescence search
Opening book
Endgame tablebases
Performance optimization

License

This project is intended for educational use and may be modified freely.
