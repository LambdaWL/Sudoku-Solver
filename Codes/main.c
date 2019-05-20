/**
 * Welcome to Seashell!
 */

#include <stdio.h>
#include "sudoku.h"
#include "file_io.h"
#include <string.h>

// main_game_loop(board, initial) returns true if all the grids have
//   been successfully filled up in *board, or false if the current
//   Sudoku puzzle has been saved
// effects: may modify *board
//          reads inputs
//          produces outputs
bool main_game_loop(int *board, bool *initial) {
  // Main Game Loop
  char command[20];
  bool game_finished = false;
  while (!game_finished) {
    printf("-----------------------------------------------------\n");
    printf(" (hint x y): display all available numbers at (x,y)  \n");
    printf("       (fill x y k): fill in value k at (x,y)        \n");
    printf("       (save): save the current Sudoku puzzle        \n");
    printf("  (restore): return the board to initial condition   \n");
    printf("-----------------------------------------------------\n");
    if (scanf("%s", command) != 1) {
      fprintf(stderr, "Command Reading Failed! Exiting...\n");
      exit(EXIT_FAILURE);
    }
    if (strcmp(command, "hint") == 0) {
      int x;
      int y;
      if (!(scanf("%d", &x) == 1) || !(scanf("%d", &y) == 1) ||
	  x > 9 || x < 1 || y > 9 || y < 1) {
        printf("Invalid arguments provided. Please try again.\n");
        continue;
      }
      sudoku_hint(board, x - 1, y - 1);
    } else if (strcmp(command, "fill") == 0) {
      int x;
      int y;
      int k;
      if (!(scanf("%d", &x) == 1) || !(scanf("%d", &y) == 1) ||
          !(scanf("%d", &k) == 1) || x > 9 || x < 1 || y > 9 || 
	  y < 1 || k > 9 || k < 1) {
        printf("Invalid arguments provided. Please try again.\n");
        continue;
      }
      sudoku_fill(board, initial, x - 1, y - 1, k);
      game_finished = sudoku_complete(board);
    } else if (strcmp(command, "save") == 0) {
      printf("Please enter the name of the saving file, ");
      printf("make sure that it is less than 10 characters:\n");
      char name[11];
      if (scanf("%s", name) != 1) {
        fprintf(stderr, "Error Occurred Reading Input. ");
        fprintf(stderr, "Exiting...");
        exit(EXIT_FAILURE);
      }
      board_save(board, name);
      return false;
    } else if (strcmp(command, "restore") == 0) {
		board_restore(board, initial);
		board_print(board, initial);
    } else {
      printf("Invalid command provided. Please try again.\n");
    }  
  }
  return true;
}


int main(void) {
  printf("---------- Welcome to Sudoku Solver (v1.0) ----------\n");
  printf("             Customized Game (customized)            \n");
  printf("              Load Existing Game (load)              \n");
  printf("                    Quit (quit)                      \n");
  printf("-----------------------------------------------------\n");
  printf("Please enter any command listed above (in the braket):\n");
  char command[20];
  if (scanf("%s", command) != 1) {
    fprintf(stderr, "Command Reading Failed! Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if (strcmp(command, "customized") == 0) {
    int *new_board = board_create();
    bool *new_initial = board_init_create(new_board);
    board_print(new_board, new_initial);
    if (main_game_loop(new_board, new_initial)) {
      printf("Congratulations! The Sudoku puzzle has been completed!\n");
      board_print(new_board, new_initial);
    } else {
      printf("Current game has been saved.\n");
      board_print(new_board, new_initial);
      printf("Thank you for using Sudoku Solver (v1.0)!\n");
      printf("Have a nice day :)\n");
    }
    free(new_board);
    free(new_initial);
  } else if (strcmp(command, "load") == 0) {
    print_files();
    printf("Please enter the name of the file you want to load, ");
    printf("make sure that it is less than 10 characters.\n");
    printf("Note: If the input name is invalid, please keep trying:\n");
    char path[30] = "./Save/";
    char name[11];
    char temp[50];
    FILE *fp = NULL;
    do {
      if (scanf("%s", name) != 1) {
        fprintf(stderr, "Error Occurred Reading Input. ");
        fprintf(stderr, "Exiting...");
        exit(EXIT_FAILURE);
      }
      strcpy(temp, path);
      strcat(temp, name);
      fp = fopen(temp, "r");
    } while (fp == NULL);
    int *new_board = board_load(fp);
    bool *new_initial = board_init_create(new_board);
	board_print(new_board, new_initial);
    if (main_game_loop(new_board, new_initial)) {
      printf("Congratulations! The Sudoku puzzle has been completed!\n");
      board_print(new_board, new_initial);
    } else {
      printf("Current game has been saved.\n");
      board_print(new_board, new_initial);
      printf("Thank you for using Sudoku Solver (v1.0)!\n");
      printf("Have a nice day :)\n");
    }
    free(new_board);
    free(new_initial);
  } else if (strcmp(command, "quit") == 0) {
    printf("Thank you for using Sudoku Solver (v1.0)!\n");
    printf("Have a nice day :)\n");
  } else {
    fprintf(stderr, "Invalid Command! Exiting...\n"); // ** more on this
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
