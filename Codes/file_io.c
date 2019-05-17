/**
 File: file_io.c
 Implementations for file i/o
*/

#include "file_io.h"


// see file_io.h
void print_files(void) {
  FILE *fp = fopen("./Save/.list_of_filenames.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error has occurred! File might be renamed ");
    fprintf(stderr, "or permanently removed! Exiting...\n");
    exit(EXIT_FAILURE);
  }
  char c;
  printf("All the saved files are:\n");
  while (fscanf(fp, "%c", &c) == 1) {
    if (c == '\n')
      printf("\n");
    else
      printf("%c", c);
  }
  printf("\n");
}


// see file_io.h
int *board_load(FILE *fp) {
  int *board = malloc(sizeof(int) * 9 * 9);
  int temp;
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      fscanf(fp, "%d", &temp);
      board[r * 9 + c] = temp;
    }
  }
  fclose(fp);
  return board;
}


// see file_io.h
void board_save(int *board, char *name) {
  // Update the list of saved files
  FILE *fp = fopen("./Save/.list_of_filenames.txt", "a"); // "a": append
  char *temp = name;
  while (*temp) {
    fprintf(fp, "%c", *temp);
    ++temp;
  }
  fprintf(fp, "\n");
  fclose(fp);
  // Create a new file
  char path[30] = "./Save/";
  strcat(path, name);
  fp = fopen(path, "w");
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      fprintf(fp, "%d", board[r * 9 + c]);
      if (c != 8)
        fprintf(fp, " ");
      else
        fprintf(fp, "\n");
    }
  }
  fclose(fp);
}
