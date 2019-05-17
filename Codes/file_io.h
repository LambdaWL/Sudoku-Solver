/**
 File: file_io.h
 Enter a description for this file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// print_files(void) prints a list of all files that have been saved
// effects: produces outputs
void print_files(void);


// board_load(fp) reads inputs from file *fp and creates a Sudoku 
//   board (caller must call free) 
// effects: allocates memory
int *board_load(FILE *fp);


// board_save(board, name) saves the current *board as [name] in
//   another directory
// effects: changes program status
void board_save(int *board, char *name);
