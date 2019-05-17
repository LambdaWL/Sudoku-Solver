/**
 File: sudoku.h
 Interface for sudoku.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// board_create(void) creates a customized Sudoku board and returns
//   the address of it; NULL is returned if creation is unsuccessful
// effects: reads inputs
//          allocates memory (caller must call free)
int *board_create(void);


// board_init_create(board) creates a board with boolean values
//   that records the initial condition of the Sudoku board
// effects: allocates memory (caller must call free)
bool *board_init_create(int *board);


// board_restore(board, initial) returns the Sudoku board to its
//   initial condition
// effects: modifies *board
void board_restore(int *board, bool *initial);


// board_print(board, initial) prints the current status of
//   the Sudoku board; * denotes the grids that are not to be
//   modified by the player
// effects: produces output
void board_print(int *board, bool *initial);


// sudoku_solver(board, initial) finds all possible to the Sudoku
//   puzzle, but the original *board will not be modified
// effects: produces outputs
void sudoku_solver(int *board, bool *initial);


// sudoku_complete(board) determines if the Sudoku puzzle has been
//   filled completely
bool sudoku_complete(int *board);


// sudoku_hint(board, r, c) provides all numbers that can be filled
//   in at the current status at (r + 1, c + 1)
// effects: produces outputs
void sudoku_hint(int *board, int r, int c);


// sudoku_fill(board, initial, r, c, val) fills val at (r + 1, c + 1)
//   provided it is possible
// effects: may modify *board
//          produces outputs
void sudoku_fill(int *board, bool *initial, int r, int c, int val);
