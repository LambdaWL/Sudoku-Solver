/**
 File: sudoku.c
 Implementations for sudoku-related functions
*/

#include "sudoku.h"


// See sudoku.h
int *board_create(void) {
  int *board = malloc(sizeof(int) * 9 * 9);
  char temp;
  printf("Please input the sudoku in the following format:\n");
  printf("1. Rows are inputted consecutively, one at a time\n");
  printf("2. Each unknown entry is inputted as zero\n");
  printf("3. Only digits are allowed\n");
  for (int r = 0; r < 9; ++r) {
    printf("Please input the %d-th row:\n", r + 1);
    for (int c = 0; c < 9; ++c) {
      // EOF is inputted (read-in ends)
      if (scanf(" %c", &temp) == -1) {
        printf("EOF detected. Sudoku board creation failed.\n");
        free(board);
        return NULL;
      }
      // Not a valid input (the inputted character is not 0-9)
      if (temp < '0' || temp >'9') {
        printf("Input is invalid! Please re-enter ");
        printf("the entries starting from ");
        printf("(%d,%d) (inclusive):\n", r + 1, c + 1);
        while (temp != '\n') {
          scanf("%c", &temp); // skipping the current line
        }
        --c;
        continue;
      }
      board[r * 9 + c] = temp - '0';
    }
  }
  return board;
}


// See sudoku.h
bool *board_init_create(int *board) {
  bool *initial = malloc(sizeof(bool) * 9 * 9);
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      initial[r * 9 + c] = (board[r * 9 + c] == 0) ? false : true;
    }
  }
  return initial;
}


// See sudoku.h
void board_restore(int *board, bool *initial) {
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (!initial[r * 9 + c])
        board[r * 9 + c] = 0;
    }
  }
}


// See sudoku.h
void board_print(int *board, bool *initial) {
  for (int r = 0; r < 9; ++r) {
    if (r % 3 == 0) 
      printf("-------------------------------\n");
    for (int c = 0; c < 9; ++c) {
      if (c % 3 == 0)
        printf("|");
      if (initial[r * 9 + c])
        printf("*");
      else
        printf(" ");
      switch (board[r * 9 + c]) {
        case 0:
          printf("  ");
          break;
        default:
          printf("%d ", board[r * 9 + c]);
      }
      if (c == 8)
        printf("|\n");
    }
  }
  printf("-------------------------------\n");
}


// STATIC
// already_exist(board, r, c, val) determies if the given value
//   can be filled at the given position in the Sudoku board
static bool already_exist(int *board, int r, int c, int val) {
  for (int i = 0; i < 9; ++i) {
    if (board[i * 9 + c] == val)
      return true;
  }
  for (int j = 0; j < 9; ++j) {
    if (board[r * 9 + j] == val)
      return true;
  }
  int r_bound = (r / 3) * 3;
  int c_bound = (c / 3) * 3;
  for (int i = r_bound; i < r_bound + 3; ++i) {
    for (int j = c_bound; j < c_bound + 3; ++j) {
      if (board[i * 9 + j] == val)
        return true;
    }
  }
  return false;
}


// STATIC
// an abstract data type that stores the solutions to Sudoku puzzles
struct solutions {
  int maxlen; // maximum number of solutions that can be contained
  int curlen; // current number of solutions being stored
  int **data; // dynamic array that stores solutions
};


// STATIC
// solutions_update(board, sol) creates a duplicate of *board and stores
//   that duplicate in *sol
// effects: allocates memory
//          modifies *sol
static void solutions_update(int *board, struct solutions *sol) {
  if (sol->maxlen == sol->curlen) {
    sol->maxlen *= 2;
    sol->data = realloc(sol->data, sol->maxlen * sizeof(int *));
  }
  int *duplicate = malloc(sizeof(int) * 9 * 9);
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      duplicate[r * 9 + c] = board[r * 9 + c];
    }
  }
  sol->data[sol->curlen] = duplicate;
  ++sol->curlen;
}


// STATIC
// sudoku_solver_help(board, r, c, sol) recursively searches for solutions
//   to the Sudoku puzzle and stores all possible solutions in a 
//   dynamically allocated structure solutions
static void sudoku_solver_help(int *board, int r, int c, 
                              struct solutions *sol) {
  if (r == 9) {
    solutions_update(board, sol);
    return;
  }
  if (c == 9) {
    sudoku_solver_help(board, r + 1, 0, sol);
    return;
  }
  if (board[r * 9 + c] != 0) {
    sudoku_solver_help(board, r, c + 1, sol);
    return;
  }
  for (int k = 1; k < 10; ++k) {
    if (already_exist(board, r, c, k))
      continue;
    board[r * 9 + c] = k;
    sudoku_solver_help(board, r, c + 1, sol);
    board[r * 9 + c] = 0;
  }
  return;
}


// See sudoku.h
void sudoku_solver(int *board, bool *initial) {
  struct solutions *sol = malloc(sizeof(struct solutions));
  sol->maxlen = 1;
  sol->curlen = 0;
  sol->data = malloc(sizeof(int *) * 1);
  // Keep track of the original board
  int *backup = malloc(sizeof(int) * 9 * 9);
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      backup[r * 9 + c] = board[r * 9 + c];
    }
  }
  sudoku_solver_help(board, 0, 0, sol);
  printf("The number of possible solutions to the Sudoku puzzle: ");
  printf("%d\n", sol->curlen);
  if (sol->curlen == 0) {
    printf("The Sudoku is likely unsolvable.\n");
  } else {
    printf("Do you want to list all of the possible solutions?\n");
    printf("[type in 1]: only list one solution\n");
    printf("[type in 2]: list all solutions\n");
    printf("[type in 3]: do not list any solution\n");
    int cmd;
    scanf("%d", &cmd);
    if (cmd == 1) {
      board_print(sol->data[0], initial);
    } else if (cmd == 2 || cmd != 3) {
      if (cmd != 2 && cmd != 3)
        printf("Invalid command. List all solutions by default.\n");
      for (int i = 0; i < sol->curlen; ++i) {
        board_print(sol->data[i], initial);
      }
    }
  }
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      board[r * 9 + c] = backup[r * 9 + c];
    }
  }
  free(backup);
  // free up the structure solutions
  for (int i = 0; i < sol->curlen; ++i) {
    free(sol->data[i]);
  }
  free(sol->data);
  free(sol);
}


// See sudoku.h
bool sudoku_complete(int *board) {
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (board[r * 9 + c] == 0)
        return false;
    }
  }
  return true;
}


// See sudoku.h
void sudoku_hint(int *board, int r, int c) {
  if (board[r * 9 + c] != 0) {
    printf("This grid has already been filled!\n");
    return;
  }
  printf("Numbers still available at (%d,%d): [", r + 1, c + 1);
  int count = 0;
  for (int k = 1; k < 10; ++k) {
    if (already_exist(board, r, c, k))
      continue;
    if (count > 0)
      printf(", ");
    printf("%d", k);
    ++count;
  }
  if (count == 0)
    printf("empty");
  printf("]\n");
}


// See sudoku.h
void sudoku_fill(int *board, bool *initial, int r, int c, int val) {
  if (board[r * 9 + c] != 0) {
    printf("This grid has already been filled!\n");
    return;
  }
  if (already_exist(board, r, c, val)) {
    printf("%d cannot be filled at (%d,%d)!\n", val, r + 1, c + 1);
    sudoku_hint(board, r, c);
    return;
  }
  board[r * 9 + c] = val;
  board_print(board, initial);
}
